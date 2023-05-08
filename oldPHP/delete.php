<?php
if (isset($_GET['filename'])) {
    $filename = $_GET['filename'];
    if (file_exists("./uploads/".$filename)) {
        unlink("./uploads/".$filename); // Suppression du fichier
        header("Location: index.php"); // Redirection vers la page principale
        exit;
    }
}
?>

<?php
	// Chemin du répertoire d'upload
	$dir = "./uploads/";

	// Vérifie si le répertoire existe
	if (is_dir($dir))
	{
		// Récupère la liste des fichiers dans le répertoire d'upload
		$files = scandir($dir);

		// Parcours la liste des fichiers
		foreach ($files as $file)
		{
			// Vérifie que le fichier n'est pas le répertoire parent ou courant
			if ($file != "." && $file != "..")
			{
				// Affichage du nom du fichier
				echo "<li>" . $file;

				// Bouton de suppression du fichier
				echo "<form action='delete.php' method='post'>";
				echo "<input type='hidden' name='filename' value='" . $file . "' >";
				echo "<input type='submit' name='delete' value='Supprimer'>";
				echo "</form></li>";
			}
		}
	} else {
		// Affichage d'un message si le répertoire n'existe pas
		echo "Aucun fichier présent dans le répertoire d'upload.";
	}
?>
