<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Page PHP avec une requete POST</title>
    <style>
        body {
            background-color: #97f15a;
            margin: 0;
            padding: 0;
        }

        header {
            position: fixed;
            top: 0;
            left: 0;
            right: 0;
            z-index: 999;
            background-color: #97f15a;
            text-align: center;
            font-size: 2em;
            font-weight: bold;
            box-shadow: 0px 2px 5px rgba(255, 255, 255, 0.3);
        }

		header a {
			color: white;
			width: 100%;
			height: 100%;
			padding: 10px 0;
			display: block;
			text-decoration: none;
		}

        footer {
            position: fixed;
            bottom: 0;
            left: 0;
            right: 0;
            background-color: #111;
            color: white;
            text-align: center;
            padding: 10px 0;
            font-size: 0.8em;
        }

        main {
			margin-top: 62px;
            padding-bottom: 60px;
        }
    </style>
</head>
<body>
	<header>
		<a href="/index.html">WebServ</a>
	</header>
	<main>
	<?php
		$data = file_get_contents('php://input');
		parse_str($data, $post_vars);
		$_POST = $post_vars;
        // Vérifie si le formulaire a été soumis
        if (isset($_GET['nom']) && isset($_GET['prenom'])) {
            // Affiche les valeurs dans les champs du formulaire
            echo '<form action="post.php" method="post">
                      <label for="nom">Nom :</label>
                      <input type="text" name="nom" id="nom" value="' . htmlspecialchars($_GET['nom']) . '">
                      <br>
                      <label for="prenom">Prénom :</label>
                      <input type="text" name="prenom" id="prenom" value="' . htmlspecialchars($_GET['prenom']) . '">
                      <br>
                      <input type="submit" value="Envoyer">
                  </form>
				  </br>';
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
    ?>
	</main>
	<footer>🐑 Site web créé par Nicolas, Mathieu et Cyril 🐑</footer>
</body>
</html>
