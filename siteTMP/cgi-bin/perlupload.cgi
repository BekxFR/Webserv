#!/usr/bin/perl

use strict;
use CGI qw(:standard);

# Récupère le fichier téléversé depuis le formulaire HTML
my $filehandle = upload('myfile');

# Vérifie que le fichier a été correctement téléversé
if (!$filehandle) {
  print header(), start_html('Erreur de téléversement'), h1('Erreur de téléversement'), p('Le fichier n\'a pas été téléversé.'), end_html();
  exit;
}

# Configure le chemin de destination pour enregistrer le fichier téléversé
my $destination = '/mnt/nfs/homes/chillion/42/GitHub/Webserv/site/upload/' . $filehandle;

# Ouvre le fichier de destination pour écriture
open(my $fh, '>', $destination) or die "Impossible d'ouvrir le fichier $destination pour écriture : $!";

# Lit le contenu du fichier téléversé en chunks de 1 Ko et l'écrit dans le fichier de destination
while (my $chunk = <$filehandle>) {
  print $fh $chunk;
}

# Ferme le fichier de destination
close($fh);

# Affiche une page HTML pour indiquer que le téléversement a réussi
print header(), start_html('Téléversement réussi'), h1('Téléversement réussi'), p('Le fichier a été téléversé avec succès.'), end_html();
