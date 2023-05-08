<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Page PHP avec arguments en POST</title>
</head>
<body>
    <?php
		$data = file_get_contents('php://input');
		parse_str($data, $post_vars);
		$_POST = $post_vars;
        // Vérifie si le formulaire a été soumis
        if (isset($_POST['nom']) && isset($_POST['prenom'])) {
            // Affiche les valeurs dans les champs du formulaire
            echo '<form action="post.php" method="post">
                      <label for="nom">Nom :</label>
                      <input type="text" name="nom" id="nom" value="' . htmlspecialchars($_POST['nom']) . '">
                      <br>
                      <label for="prenom">Prénom :</label>
                      <input type="text" name="prenom" id="prenom" value="' . htmlspecialchars($_POST['prenom']) . '">
                      <br>
                      <input type="submit" value="Envoyer">
                  </form>';
				  echo "<p>Welcome " . $_GET['nom'] . " " . $_GET['prenom'] . " to the fabulous word of Webserv 🐑</p>";
        } else {
            // Affiche le formulaire vide
            echo '<form action="post.php" method="post">
                      <label for="nom">Nom :</label>
                      <input type="text" name="nom" id="nom">
                      <br>
                      <label for="prenom">Prénom :</label>
                      <input type="text" name="prenom" id="prenom">
                      <br>
                      <input type="submit" value="Envoyer">
                  </form>';
        }
        
        // Affiche les paramètres de la requête
        echo "<p>Paramètres de la requête : " . $_SERVER['QUERY_STRING'] . "</p>";
		echo "<p>" . var_dump($_POST) . "</p>";
		echo "<p>" . var_dump($data) . "</p>";
		$nom = $_POST['nom'];
		echo "5555";
		echo $data;
		echo "6666";
		echo "<p>" . var_dump($nom) . "</p>";
		echo "<p>" . var_dump($_POST['nom']) . "</p>";
		echo "<p>" . var_dump($_POST['prenom']) . "</p>";
		echo "<p>" . var_dump($_GET['nom']) . "</p>";
		echo "<p>" . var_dump($_GET['prenom']) . "</p>";
		echo "<p>" . var_dump($_SERVER['CONTENT_TYPE']) . "</p>";
    ?>
</body>
</html>
