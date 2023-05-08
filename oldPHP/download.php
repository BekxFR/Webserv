<?php
	$filename = $_GET['file'];
	$file = "./uploads/" . $filename;

	// Vérifier si le fichier existe
	if (file_exists($file)) {
		// Spécifier le type MIME correct pour le type de fichier
		header('Content-type: application/octet-stream');
		// Spécifier le nom du fichier lors du téléchargement
		header("Content-Disposition: attachment; filename=\"$filename\"");
		// Lire le contenu du fichier et l'envoyer au navigateur
		readfile($file);
	} else {
		// Si le fichier n'existe pas, afficher un message d'erreur
		echo "Le fichier demandé n'existe pas.";
	}
?>
