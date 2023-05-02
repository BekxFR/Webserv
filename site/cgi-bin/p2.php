<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Page PHP avec arguments en POST et cookies</title>
</head>
<body>
    <?php
    // Vérifie si l'utilisateur est authentifié
    $user_id = isset($_COOKIE['user_id']) ? $_COOKIE['user_id'] : null;
    $is_authenticated = !empty($user_id);

    // Si l'utilisateur est authentifié, affiche les valeurs des cookies
    if ($is_authenticated && isset($_COOKIE['nom']) && isset($_COOKIE['prenom'])) {
        echo '<h2>Les informations suivantes sont stockées dans les cookies :</h2>';
        echo '<p>Nom : ' . htmlspecialchars($_COOKIE['nom']) . '</p>';
        echo '<p>Prénom : ' . htmlspecialchars($_COOKIE['prenom']) . '</p>';
    }

    // Si le formulaire a été soumis, enregistre les valeurs dans les cookies
    if (isset($_POST['nom']) && isset($_POST['prenom'])) {
        // Enregistre les valeurs dans les cookies
        setcookie('nom', $_POST['nom'], time() + 3600 * 24 * 365); // durée de vie : 1 an
        setcookie('prenom', $_POST['prenom'], time() + 3600 * 24 * 365);

        // Si l'utilisateur n'est pas déjà authentifié, génère un identifiant aléatoire et le stocke dans un cookie
        if (!$is_authenticated) {
            $user_id = uniqid();
            setcookie('user_id', $user_id, time() + 3600 * 24 * 365);
        }

        // Affiche les valeurs dans les champs du formulaire
        echo '<form action="p2.php" method="post">
                  <label for="nom">1Nom :</label>
                  <input type="text" name="nom" id="nom" value="' . htmlspecialchars($_POST['nom']) . '">
                  <br>
                  <label for="prenom">1Prénom :</label>
                  <input type="text" name="prenom" id="prenom" value="' . htmlspecialchars($_POST['prenom']) . '">
                  <br>
                  <input type="submit" value="Envoyer">
              </form>';
    } else {
        // Affiche le formulaire avec les valeurs des cookies si l'utilisateur est authentifié
        if ($is_authenticated && isset($_COOKIE['nom']) && isset($_COOKIE['prenom'])) {
            echo '<form action="p2.php" method="post">
                      <label for="nom">2Nom :</label>
                      <input type="text" name="nom" id="nom" value="' . htmlspecialchars($_COOKIE['nom']) . '">
                      <br>
                      <label for="prenom">2Prénom :</label>
                      <input type="text" name="prenom"  id="prenom" value="' . htmlspecialchars($_COOKIE['prenom']) . '">
					  <br>
					  <input type="submit" value="Envoyer">
				  </form>';
		} else {
			// Affiche le formulaire vide
			echo '<form action="p2.php" method="post">
					  <label for="nom">3Nom :</label>
					  <input type="text" name="nom" id="nom">
					  <br>
					  <label for="prenom">3Prénom :</label>
					  <input type="text" name="prenom" id="prenom">
					  <br>
					  <input type="submit" value="Envoyer">
				  </form>';
		}
	}
	?>
</body>
</html>