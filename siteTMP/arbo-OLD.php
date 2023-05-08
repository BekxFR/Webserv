<!DOCTYPE html>
<html>
  <head>
	<meta charset="utf-8">
    <title>Liste des fichiers</title>
    <style>
      body {
        background-color: white;
      }
    </style>
  </head>
  <body>
    <h1>Liste des fichiers</h1>
    <?php
      // Récupérer le répertoire courant
      $current_dir = realpath('.');
      
      // Vérifier si on doit changer de répertoire 
      if(isset($_GET['dir'])) {
        $new_dir = realpath($_GET['dir']);
        if($new_dir !== false && is_dir($new_dir)) {
          chdir($new_dir);
          $current_dir = $new_dir;
        }
      }

	  
      // Récupérer la liste des fichiers du répertoire courant
      $files = scandir('.');
      
      // Afficher la liste des fichiers
      echo '<ul>';
		  // Afficher le lien pour remonter d'un niveau si nécessaire
		  if($current_dir != '/') {
			echo '<li><a href="?dir=' . urlencode(dirname($current_dir)) . '">..</a><br></li>';
		  }
		  foreach($files as $file) {
        if($file != '.' && $file != '..') {
          if(is_dir($file)) {
            echo '<li><a href="?dir=' . urlencode($file) . '">' . $file . '</a></li>';
          } else {
            echo '<li>' . $file;
            echo ' - Droits : ' . substr(sprintf('%o', fileperms($file)), -4);
            echo ' - Créé le : ' . date('F d Y H:i:s.', filectime($file));
            echo ' - <a href="' . $file . '" download>Télécharger</a>';
            echo ' - <a href="?delete='.urlencode($file).'" onclick="return confirm(\'Êtes-vous sûr de vouloir supprimer ce fichier ?\')">Supprimer</a>';
            echo '</li>';
          }
        }
      }

      // Supprimer le fichier si nécessaire
      if(isset($_GET['delete'])) {
        $filename = urldecode($_GET['delete']);
        if(file_exists($filename)) {
          unlink($filename);
          echo '<p>Le fichier ' . $filename . ' a été supprimé avec succès.</p>';
        } else {
          echo '<p>Le fichier ' . $filename . ' n\'existe pas.</p>';
        }
      }
      
      echo '</ul>';
    ?>
  </body>
</html>
