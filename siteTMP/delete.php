<?php
if(isset($_GET['file'])){
    $file = 'uploads/'.$_GET['file'];
    if(file_exists($file)){
        unlink($file);
        echo 'Le fichier a bien été supprimé.';
    }
}
?>
