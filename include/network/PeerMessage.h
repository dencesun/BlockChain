//
// Created by dencesun on 20/03/2018.
//

#ifndef BLOCKCHAIN_PEERMESSAGE_H
#define BLOCKCHAIN_PEERMESSAGE_H

#include <string>

using std::string;

namespace MessageIHM {
    const string introMessage = "--- BLockchaiN ---";
    const string formulation_demande_identite = "Veuillez entre un nom puis un prenom ?";
    const string formulation_demande_nom = "Nom :";
    const string formulation_demande_prenom = "Prenom :";
    const string formulation_demande_possession_cle = "Avez vous une paire de cle existente : [y:n]";
    const string formulation_demande_location_cle_publique = "Precisez l'endroit ou se trouve votre cle publique :";
    const string formulation_demande_location_cle_privee = "Precisez l'endroit ou se trouve votre cle privee :";
    const string information_generation_cle = "une paire de cle va etre genere";
    const string formulation_sauvegarde_cle = "voulez vous sauvegardez vos cle [y:n]";
    const string infomation_cle_sauvegarde = "Vos cles ont ete sauvegarde";

    const string affichage_menu_principal =
            "- 1. Creer et enregistrer dans la BlockChain un nouveau nom [1]\n- 2. Commencer a miner [2] \n- 3. Changer d\'identite \n";
    const string formulation_demande_creation_transaction_sous_idenite = "Voulez vous creer une transaction sous cet identifiant";
    const string y_or_n = "[y-n]";
    const string formulation_demande_nom_de_domaine = "Veuillez rentrez le nom de domaine auquel vous souhitez vous connectez";
    const string formulation_demande_information = "Veuillez rentrez les informations que vous souhaitez ajouter concernant le nom de domaine, le tout SUR UNE LIGNE";
    const string error_while_creating_transaction = "Sorry but the transaction has failed to be created !";
}
#endif //BLOCKCHAIN_PEERMESSAGE_H
