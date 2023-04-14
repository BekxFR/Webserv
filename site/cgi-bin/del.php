<?php
if ($_SERVER['REQUEST_METHOD'] === 'DELETE') {
    $file_path = '/usr/share/nginx/html/upload/test.txt';
    if (file_exists($file_path)) {
        unlink($file_path);
        echo "Le fichier a été supprimé avec succès.";
    } else {
        echo "Le fichier n'existe pas.";
    }
}
?>

<html>
<head>
    <title>Supprimer un fichier</title>
</head>
<body>
    <form method="DELETE">
        <button type="submit">Supprimer le fichier</button>
    </form>
</body>
</html>
