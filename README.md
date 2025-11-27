# Projet Déneigeuse-Souffleuse FDN-P23-71
Pour le projet PIRUS, l’équipe P-23 en génie robotique, de l'université de Sherbrooke, a créé un prototype d’un robot étant capable de déneiger plusieurs entrées des maisons de façon autonome.

**Équipe : P23**  
**Auteurs : Karina Ait Ouazzou, Étienne Vibert**  
**Date : 2025-11-06**
 
---
 
## Description générale
 
Ce projet contient le code principal du robot déneigeur autonome.  
Le robot utilise :
 
- des capteurs suiveurs de ligne,
- un capteur de distance (GP2D12),
- deux moteurs contrôlés par LibRobus,
- une pelle motorisée,
- une souffleuse contrôlée par ESC,
- de la logique embarquée pour suivre une ligne, détecter une maison, déneiger devant la maison, puis revenir sur la ligne.
 
Le fichier central est `main.cpp`.
 
---
 
## Fichiers du projet
 
    main.cpp
    movement.h
    suiveurDeLigne.h
    detectionBouton.h
    pelleSouffleuse.h
    pin.h
 
Librairies utilisées :
 
- `LibRobus.h` (moteurs, encodeurs, servos…)
- `GP2D12.h` (capteur de distance)
- `Arduino.h`
 
---
 
## Initialisation (setup)
 
À l’allumage, le programme exécute les étapes suivantes :
 
1. Initialisation de la carte :
 
        BoardInit();
 
2. Coupure du buzzer :
 
        digitalWrite(pinBuzzer, LOW);
 
3. Position initiale de la pelle (fermée / en bas) :
 
        pelle(ETEIN);
 
4. Activation des servos (pelle et ESC de la souffleuse) :
 
        SERVO_Enable(0);
        SERVO_Enable(ESC_pin);
        SERVO_SetAngle(ESC_pin, ESC_MIN);
 
5. Arrêt des moteurs et remise à zéro des encodeurs :
 
        MOTOR_SetSpeed(0, 0);
        MOTOR_SetSpeed(1, 0);
        ENCODER_Reset(0);
        ENCODER_Reset(1);
 
6. Démarrage de la communication série pour le débogage :
 
        Serial.begin(115200);
 
---
 
## Fonctionnement principal (loop)
 
La boucle `loop()` contient la logique complète du robot. Elle tourne en permanence.
 
1. Le robot suit la ligne **en reculant** :
 
        suivreLigneRecule();
 
2. À chaque itération, il vérifie la distance mesurée par le capteur GP2D12 à gauche.  
   Si une "maison" est détectée à moins de 30 cm :
 
        if (DistanceGauche() < 30)
 
3. Dès qu’une maison est détectée, le robot cherche une **ligne horizontale** (ligne perpendiculaire) pour se positionner correctement devant la maison :
 
        while (!ligneHorizontale()) {
            suivreLigneRecule();
        }
 
4. Une fois la ligne horizontale trouvée, le robot :
 
   - s’arrête :
 
            MOTOR_SetSpeed(0, 0);
            MOTOR_SetSpeed(1, 0);
 
   - active le buzzer pour signaler qu’il a trouvé la maison :
 
            digitalWrite(pinBuzzer, HIGH);
 
   - ouvre (monte) la pelle :
 
            pelle(ALLUMER);
            delay(8000);   // temps pour un mouvement complet
 
   - allume la souffleuse :
 
            souffleuse(ALLUMER);
 
   - avance pour déneiger devant la maison :
 
            avancer_MOVE(150, -0.3);
 
   - effectue une rotation sur place vers la gauche pour se réaligner sur un autre tronçon de ligne :
 
            surplace_GAUCHE(45, 0.3);
            tournerLigneGauche();
 
   - continue à reculer jusqu’à retrouver une nouvelle ligne horizontale :
 
            while (!ligneHorizontale()) {
                suivreLigneRecule();
            }
 
   - s’arrête à nouveau, ferme la pelle et lui laisse le temps de terminer le mouvement :
 
            MOTOR_SetSpeed(0, 0);
            MOTOR_SetSpeed(1, 0);
            pelle(ETEIN);
            delay(8000);
 
   - coupe le buzzer :
 
            digitalWrite(pinBuzzer, LOW);
 
   - recule un peu pour se dégager :
 
            reculer_MOVE(100, -0.3);
 
   - avance en suivant la ligne jusqu’à retrouver encore une ligne horizontale :
 
            while (!ligneHorizontale()) {
                suivreLigne();
            }
 
   - éteint la souffleuse :
 
            souffleuse(ETEIN);
 
   - avance légèrement :
 
            avancer_MOVE(100, -0.3);
 
   - tourne sur place vers la droite, puis se réaligne sur la ligne grâce à la fonction dédiée :
 
            surplace_DROIT(45, 0.3);
            tournerLigneDroite();
 
5. La boucle recommence ensuite : le robot retourne en mode suivi de ligne en reculant, et attend de détecter une nouvelle maison.
 
---
 
## Fonctions de rotation vers la ligne
 
### `tournerLigneDroite()`
 
Effectue une rotation sur place vers la droite jusqu’à ce que le capteur central détecte la ligne.
 
    void tournerLigneDroite()
    {
      float angle = 0.0;
      while (!(detectionLigne(CAPTEUR_CENTRE, seuilContrasteCentre) == true))
      {
        MOTOR_SetSpeed(0, 0.2);
        MOTOR_SetSpeed(1, -0.2);
      }
    }
 
### `tournerLigneGauche()`
 
Même principe, mais vers la gauche.
 
    void tournerLigneGauche()
    {
      float angle = 0.0;
      while (!(detectionLigne(CAPTEUR_CENTRE, seuilContrasteCentre) == true))
      {
        MOTOR_SetSpeed(0, -0.2);
        MOTOR_SetSpeed(1, 0.2);
      }
    }
 
Le seuil de contraste pour la détection centrale est défini par :
 
    static float seuilContrasteCentre = 100.00;
 
---
 
## Gestion de la pelle
 
La pelle est contrôlée par les fonctions définies dans `pelleSouffleuse.h`.  
Les appels principaux vus dans `main.cpp` sont :
 
- pour monter/ouvrir la pelle :
 
        pelle(ALLUMER);
 
- pour descendre/fermer la pelle :
 
        pelle(ETEIN);
 
Un délai d’environ 8 secondes est utilisé pour laisser le temps à la pelle de compléter son mouvement :
 
    delay(8000);
 
---
 
## Gestion de la souffleuse (ESC)
 
La souffleuse est pilotée via un ESC commandé en PWM comme un servo.  
L’initialisation met l’ESC à l’angle minimal :
 
    SERVO_Enable(ESC_pin);
    SERVO_SetAngle(ESC_pin, ESC_MIN);
 
Les fonctions appelées dans la logique principale sont :
 
- allumer la souffleuse :
 
        souffleuse(ALLUMER);
 
- éteindre la souffleuse :
 
        souffleuse(ETEIN);
 
Ces fonctions sont encapsulées dans `pelleSouffleuse.h`.
 
---
 
## Capteur de distance (GP2D12)
 
Le robot utilise un capteur GP2D12 afin de détecter la présence d’une maison sur le côté gauche.  
La fonction principale est :
 
    DistanceGauche()
 
Dans la logique principale, le seuil est fixé à 30 cm :
 
    if (DistanceGauche() < 30)
    {
        // Séquence de déneigement
    }
 
Des fonctions de test sont également prévues (commentées) pour afficher la distance sur le port série.
 
---
 
## Blocs de test présents dans main.cpp
 
Le fichier `main.cpp` contient plusieurs sections de test commentées, utiles pour vérifier séparément certains modules.
 
### Test du détecteur de distance
 
Boucle simple qui affiche si une maison est détectée ou non, ainsi que la distance :
 
    if (DistanceGauche()< 30)
    {
        Serial.println("Maison detectee");
    }
    else {
        Serial.println("A la recherche de la maison!!!");
    }
 
    PrintDistance();
    delay(1000);
 
### Test de la souffleuse
 
Permet de faire tourner la souffleuse par séquences :
 
    while (true)
    {
        souffleuse(ALLUMER);
        delay(3000);
 
        souffleuse(ETEIN);
        delay(2000);
    }
 
### Test de la pelle via le terminal série
 
Une boucle lit les commandes envoyées par le port série et permet de contrôler la pelle :
 
- `u` ou `U` : monter la pelle  
- `d` ou `D` : descendre la pelle  
- `s` ou `S` : arrêter la pelle  
- `h` ou `H` : afficher l’aide
 
Extrait :
 
    if (Serial.available() > 0)
    {
        char cmd = Serial.read();
 
        switch (cmd)
        {
        case 'u':
        case 'U':
            Serial.println(F("Monter la pelle..."));
            pelle(ALLUMER);
            delay(8000);
            pelleArret();
            Serial.println(F("Pelle en haut."));
            break;
 
        case 'd':
        case 'D':
            Serial.println(F("Descendre la pelle..."));
            pelle(ETEIN);
            delay(8000);
            pelleArret();
            Serial.println(F("Pelle en bas."));
            break;
 
        case 's':
        case 'S':
            Serial.println(F("Stop pelle."));
            pelleArret();
            break;
 
        case 'h':
        case 'H':
            Serial.println(F("Commandes :"));
            Serial.println(F("  u = monter la pelle"));
            Serial.println(F("  d = descendre la pelle"));
            Serial.println(F("  s = stop"));
            Serial.println(F("  h = aide"));
            break;
 
        default:
            Serial.print(F("Commande inconnue : "));
            Serial.println(cmd);
            Serial.println(F("Tape 'h' pour l'aide."));
            break;
        }
    }
 
Une ancienne version plus longue de la logique globale du robot (suivi de ligne, détection maison à 50 cm, séquence de déneigement différente, etc.) est également conservée en commentaire à la fin du fichier. Elle peut servir de référence ou de base pour des tests.
 
---
 
## Compilation et téléversement
 
Prérequis :
 
- Environnement Arduino ou PlatformIO configuré
- Librairie LibRobus installée
- Robot compatible (plateforme Robus / carte supportée)
- Tous les fichiers `.h` présents dans le projet
 
Étapes générales :
 
1. Ouvrir le projet contenant `main.cpp` et les fichiers `.h`.
2. Vérifier que la librairie LibRobus est correctement installée.
3. Compiler le projet.
4. Brancher le robot et téléverser le binaire.
5. Placer le robot au début de la ligne de la piste.
6. Mettre le robot sous tension et observer son comportement.
 
---
 
## Auteurs
 
- **Karina Ait Ouazzou** – logique de détection, suivi de ligne, intégration capteurs.
- **Étienne Vibert** – déplacement, contrôle des actionneurs (pelle, souffleuse), intégration mécatronique.
 
---
 
## Référence
 
README généré à partir du fichier `main.cpp` du projet du robot déneigeur.
