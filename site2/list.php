<!DOCTYPE html>
<html>
<head>
	<title>Liste des fichiers</title>
</head>
<body>
	<h1>Liste des fichiers dans le répertoire courant :</h1>
	<ul>
		<?php
			// Ouvre le répertoire courant
			$dir = opendir(".");
			
			// Parcourt tous les fichiers du répertoire
			while (false !== ($file = readdir($dir))) {
				// Exclut les fichiers cachés (commençant par un point)
				if ($file != "." && $file != ".." && substr($file, 0, 1) != ".") {
					// Ajoute un élément de liste pour chaque fichier trouvé
					echo "<li><a href='$file'>$file</a></li>";
				}
			}
			
			// Ferme le répertoire
			closedir($dir);
		?>
	</ul>
</body>
</html>
