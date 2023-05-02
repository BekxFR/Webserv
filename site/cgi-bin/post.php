<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Page PHP avec arguments en POST</title>
</head>
<body>
    <?php
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
</body>
</html>