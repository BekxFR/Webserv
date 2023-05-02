<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Page PHP avec arguments en GET</title>
</head>
<body>
    <?php
        // Vérifie si le formulaire a été soumis
        if (isset($_GET['nom']) && isset($_GET['prenom'])) {
            // Affiche les valeurs dans les champs du formulaire
            echo '<form action="get.php" method="get">
                      <label for="nom">Nom :</label>
                      <input type="text" name="nom" id="nom" value="' . htmlspecialchars($_GET['nom']) . '">
                      <br>
                      <label for="prenom">Prénom :</label>
                      <input type="text" name="prenom" id="prenom" value="' . htmlspecialchars($_GET['prenom']) . '">
                      <br>
                      <input type="submit" value="Envoyer">
                  </form>';
        } else {
            // Affiche le formulaire vide
            echo '<form action="get.php" method="get">
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
		echo "<p>" . var_dump($_GET['nom']) . "</p>";
		echo "<p>" . var_dump($_GET['prenom']) . "</p>";
    ?>
</body>
</html>
