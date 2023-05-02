<?php
$uploadDir = './upload/';

if (!empty($_FILES['file'])) {
    $file = $_FILES['file'];

    // Vérifier s'il y a une erreur lors de l'upload
    if ($file['error'] !== UPLOAD_ERR_OK) {
        echo '<script>alert("Erreur lors du téléchargement : ' . $file['error'] . '");window.location.replace("index.php");</script>';
        exit;
    }

    // Vérifier le type de fichier
    $allowedTypes = ['image/jpeg', 'image/png', 'application/pdf', 'application/zip', 'text/plain', 'application/octet-stream'];
    if (!in_array($file['type'], $allowedTypes)) {
        echo '<script>alert("Type de fichier non autorisé");window.location.replace("index.php");</script>';
        exit;
    }

    // Vérifier la taille du fichier
    $maxFileSize = 1048576; // 1 Mo
    if ($file['size'] > $maxFileSize) {
        echo '<script>alert("Fichier trop volumineux");window.location.replace("index.php");</script>';
        exit;
    }

    // Renommer le fichier pour éviter les conflits de noms
    $fileName = $file['name'];
    $filePath = $uploadDir . $fileName;

    // Déplacer le fichier dans le répertoire d'upload
    if (!move_uploaded_file($file['tmp_name'], $filePath)) {
        echo '<script>alert("Erreur lors de la sauvegarde du fichier");window.location.replace("index.php");</script>';
        exit;
    }

    echo '<script>alert("Le fichier a été téléchargé avec succès");window.location.replace("index.php");</script>';
    exit;
}
?>
