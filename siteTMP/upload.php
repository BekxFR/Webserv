<?php

if(isset($_FILES['file'])){
	$dir = "uploads/";
	$file = $dir.basename($_FILES["file"]["name"]);
		
	if(move_uploaded_file($_FILES["file"]["tmp_name"], $file))
	{
		echo "Le fichier ".basename($_FILES["file"]["name"])." a bien été uploadé.";
	}
	else
	{
		echo "Il y a eu une erreur lors de l'upload du fichier, veuillez réessayer.";
	}
}

?>