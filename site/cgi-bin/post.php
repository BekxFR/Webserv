<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Page PHP avec arguments en POST</title>
</head>
<body>
    <?php
    // Vérifie si le formulaire a été soumis
    if (isset($_POST['nom'])) {
        // Affiche les valeurs dans les champs du formulaire
        echo '<form action="post.php" method="post">
                  <label for="nom">1Nom :</label>
                  <input type="text" name="nom" id="nom" value="' . htmlspecialchars($_POST['nom']) . '">
                  <br>
                  <input type="submit" value="Envoyer">
              </form>';
    } else {
        // Affiche le formulaire vide
        echo '<form action="post.php" method="post">
                  <label for="nom">2Nom :</label>
                  <input type="text" name="nom" id="nom">
                  <br>
                  <input type="submit" value="Envoyer">
              </form>';
    }
    ?>
</body>
</html>