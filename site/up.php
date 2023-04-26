<?php
$uploadDir = './upload/';

if (!empty($_FILES['file'])) {
    $file = $_FILES['file'];

    // Vérifier s'il y a une erreur lors de l'upload
    if ($file['error'] !== UPLOAD_ERR_OK) {
        die('Erreur lors du téléchargement : ' . $file['error']);
    }

    // Vérifier le type de fichier
    $allowedTypes = ['image/jpeg', 'image/png', 'application/pdf', 'application/zip', 'text/plain', 'application/octet-stream'];
    if (!in_array($file['type'], $allowedTypes)) {
        die('Type de fichier non autorisé');
    }

    // Vérifier la taille du fichier
    $maxFileSize = 1048576; // 1 Mo
    if ($file['size'] > $maxFileSize) {
        die('Fichier trop volumineux');
    }

    // Renommer le fichier pour éviter les conflits de noms
    $fileName = $file['name'];
    $filePath = $uploadDir . $fileName;

    // Déplacer le fichier dans le répertoire d'upload
    if (!move_uploaded_file($file['tmp_name'], $filePath)) {
        die('Erreur lors de la sauvegarde du fichier');
    }

    echo 'Le fichier a été téléchargé avec succès : <a href="' . $filePath . '">Télécharger</a>';
}

?>
