#!/usr/bin/perl

use strict;
use CGI qw(:standard);

# Récupère le nom du fichier à télécharger
my $filename = param('filename');

# Vérifie que le fichier existe et est accessible
if (-e $filename && -r $filename) {

  # Configure les en-têtes de la réponse HTTP pour indiquer qu'il s'agit d'un fichier à télécharger
  print header(-type => 'application/octet-stream',
               -attachment => $filename);

  # Ouvre le fichier et lit son contenu pour l'envoyer dans la réponse HTTP
  open(my $fh, '<', $filename) or die "Impossible d'ouvrir le fichier $filename : $!";
  while (my $line = <$fh>) {
    print $line;
  }
  close($fh);
}
else {
  # Si le fichier n'existe pas ou n'est pas accessible, renvoie une erreur 404
  print header(-status => '404 Not Found');
  print "Fichier non trouvé : $filename";
}
