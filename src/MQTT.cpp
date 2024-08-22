#include <Arduino.h>
#include <mqtt_client.h>

// Fonction pour établir une connexion MQTT
bool mqttConnect(){   
    // Indique que nous ne sommes pas connectés en faisant clignoter une LED
    ledblink(2, 70);    

    // Vérifie si le broker MQTT est configuré
    if (Mqtt_Broker[0] == '\0' || Mqtt_Broker[0] == '0'){
        Mqtt_Format = 0;  // Annule les tentatives si aucun broker n'est configuré
        return false;
    }

    // Affiche un message de diagnostic si activé
    if (diagNose != 0){
        consoleOut("Tentative de connexion à MQTT sur " + String(Mqtt_Broker));
    }

    // Définit le port par défaut si non spécifié
    if (Mqtt_Port[0] == '\0'){
        strcpy(Mqtt_Port, "1883");
    }

    // Nombre de tentatives de connexion
    uint8_t retry = 3;

    // Génère un ID unique pour l'appareil, utile si plusieurs dispositifs similaires sont utilisés
    String clientId = getChipId(false);
    
    // Boucle de tentative de connexion
    while (!MQTT_Client.connected()){

        // Tente de se connecter au broker MQTT
        if (MQTT_Client.connect(clientId.c_str(), Mqtt_Username, Mqtt_Password)){ 
            if (diagNose != 0){
                consoleOut("Connexion MQTT établie avec l'ID : " + clientId);
            }

            // Si la connexion est réussie, on s'abonne au sujet défini
            MQTT_Client.subscribe(Mqtt_inTopic); 
            if (diagNose != 0){
                consoleOut("Abonné au sujet MQTT : " + String(Mqtt_inTopic));
            }

            // Mise à jour du log pour indiquer la connexion réussie
            Update_Log("mqtt", "connected");
            return true;
        } 

        // Attente avant une nouvelle tentative
        delay(500);

        // Si toutes les tentatives échouent
        if (!--retry){
            // Mise à jour du log pour indiquer l'échec de la connexion
            Update_Log("mqtt", "connection failed");
            if (diagNose != 0){
                consoleOut("Échec de la connexion MQTT");
            }
            break;
        }
    }

    // Si on arrive ici, la connexion a échoué après plusieurs tentatives
    return false;
}

// *************************************************************************
//                  Fonction pour traiter les messages MQTT reçus
// *************************************************************************

void MQTT_Receive_Callback(char *topic, byte *payload, unsigned int length){
    // Conversion de la charge utile en chaîne de caractères
    String Payload = "";     
    for (int i = 0; i < length; i++){
        Payload += (char)payload[i]; 
    }
    
    // Affiche la charge utile reçue si le diagnostic est activé
    if (diagNose != 0){
        consoleOut("Message MQTT reçu : " + Payload);
    }

    // Utilisation de la bibliothèque JSON pour parser la charge utile
    StaticJsonDocument<1024> doc; // Document JSON pour stocker les données parsées
    DeserializationError error = deserializeJson(doc, Payload); // Désérialisation du JSON

    // Vérifie si le parsing a réussi
    if (error){
      if (diagNose != 0){
          consoleOut("JSON reçu non valide");
      }
      return;
    } 
    
    // Vérifie si le message contient une commande "poll"
    if (doc.containsKey("poll")){
      // Si le polling n'est pas déjà en cours
      if (!Polling){
        int inv = doc["poll"].as<int>(); // Récupère le numéro de l'inverter

        // Si "poll" est égal à 99, on commence le polling de tous les inverters
        if (inv == 99){
            if (diagNose != 0){
                consoleOut("Commande poll trouvée : {\"poll\" : " + String(inv) + "}");
            }
            actionFlag = 48; 
            return;  
        }

        // Vérifie si l'inverter est valide et lance le polling
        if (inv < inverterCount){ 
            actionFlag = 47; 
            return;
        } else {
            if (diagNose != 0){
                consoleOut("Erreur MQTT : Inverter non valide " + String(inv));
            }
            return;         
        }
      } else {
          if (diagNose != 0){
              consoleOut("Polling automatique en cours, commande ignorée");
          }
      }
    }
}
