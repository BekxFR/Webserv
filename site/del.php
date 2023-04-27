<?php
$file = $_GET['file'];

if (unlink('upload/' . $file)) {
    echo 'Le fichier a été supprimé.';
} else {
    echo 'Erreur lors de la suppression du fichier.';
}
?>
