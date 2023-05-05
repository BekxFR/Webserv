//////////////// 
// Pour télécharger un fichier envoyé via une requête POST, votre serveur web en C++98 doit lire les données envoyées dans le corps de la requête et les écrire dans un fichier. Pour savoir si le fichier a été reçu entièrement, vous pouvez comparer la longueur du fichier avec la longueur indiquée dans l'en-tête Content-Length de la requête HTTP.

// Voici un exemple de code qui peut être utilisé pour traiter une requête POST contenant des données binaires dans le corps de la requête :

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

void handle_post_request(int client_socket) {

    // Récupérer les données de la requête POST
    std::stringstream response;
    const int buffer_size = 1024;
    char buffer[buffer_size];
    std::memset(buffer, 0, buffer_size);
    ssize_t bytes_received = recv(client_socket, buffer, buffer_size, 0);

    // Extraire le contenu binaire
    std::string raw_data(buffer, bytes_received);
    size_t begin_content = raw_data.find("\r\n\r\n") + 4;
    std::string binary_data(raw_data.begin() + begin_content, raw_data.end());

    //Ecrire les Données dans un fichier
    std::ofstream output_file("file.bin", std::ios::out | std::ios::binary);
    output_file.write(binary_data.c_str(), binary_data.size());
    output_file.close();

    // Vérifier que toutes les données ont été reçues
    int content_length_pos = raw_data.find("Content-Length:");
    if (content_length_pos != std::string::npos) {
        size_t end_pos = raw_data.find("\r\n", content_length_pos);
        std::string content_length_str = raw_data.substr(content_length_pos + strlen("Content-Length:"), end_pos - content_length_pos - strlen("Content-Length:"));
        int expected_length = std::stoi(content_length_str);
        if (expected_length != binary_data.size()) {
            std::cout << "Erreur : Taille incomplète des données binaires" << std::endl;
        }
    }

    // Envoyer une réponse réussie au client
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: text/plain; charset=UTF-8\r\n";
    response << "Content-Length: 10\r\n";
    response << "\r\n";
    response << "Succès !\r\n";
    std::string response_str = response.str();
    send(client_socket, response_str.c_str(), response_str.length(), 0);
}

// Dans cet exemple de code, les données binaires sont extraites du corps de la requête POST à partir de la position de la séquence ```\r\n\r\n```. Ensuite, elles sont écrites dans un fichier nommé ```file.bin```. Le programme vérifie également si la taille des données binaires correspond à la longueur indiquée dans l'en-tête Content-Length de la requête POST. Si ce n'est pas le cas, il affiche une erreur. Finalement, une réponse avec un code d'état 200 est envoyée au client pour indiquer que la requête POST a réussi.

//////////////// 
Pour différencier une requête POST contenant un identifiant et mot de passe à tester dans une base de données et une requête POST demandant l'upload d'un fichier, vous pouvez analyser l'en-tête "Content-Type" de la requête HTTP.

Dans le cas où il s'agit d'une requête POST pour envoyer un fichier, l'en-tête Content-Type aura la valeur "multipart/form-data". Dans ce cas, les données du formulaire seront contenues dans le corps de la requête, divisées en plusieurs parties, chacune ayant son propre en-tête Content-Type. L'une de ces parties contiendra le fichier que vous souhaitez télécharger.

Dans le cas où il s'agit d'une requête POST contenant un identifiant et un mot de passe, l'en-tête Content-Type aura la valeur "application/x-www-form-urlencoded". Les données du formulaire seront également contenues dans le corps de la requête, mais dans ce cas, elles auront été encodées en tant que chaîne de caractères.

En analysant donc l'en-tête Content-Type, vous pouvez différencier les deux types de requêtes POST et traiter chaque type de manière appropriée.

////////////////  EN TETE REQUETE POST Content-Type
L'en-tête Content-Type indique le type de données envoyées dans la requête ou dans la réponse. Voici une liste non exhaustive des types MIME (Multipurpose Internet Mail Extension) qui peuvent être utilisés avec cette en-tête et leur signification :

application/json : données JSON ;
application/xml : données XML ;
application/pdf : fichier PDF ;
image/gif : image GIF ;
image/jpeg : image JPEG ;
image/png : image PNG ;
text/html : page HTML ;
text/plain : texte brut ;
multipart/form-data : utilisé pour envoyer des fichiers sur un serveur.
Il est important de noter que l'utilisation de l'en-tête Content-Type varie selon les besoins de l'application web, il existe donc de nombreux autres types MIME qui peuvent être utilisés.

////////////////  EN TETE REQUETE GET
Lors d'une requête GET, vous pouvez recevoir différents en-têtes HTTP en réponse. Les en-têtes que vous pouvez recevoir sont :

- `Accept-Ranges` : indique si l'entité peut être satisfaite avec une partie de la ressource.
- `Cache-Control` : spécifie les directives de gestion de cache pour les réponses.
- `Content-Encoding` : le type de codage de contenu utilisé pour compresser le corps de la réponse s'il est compressé.
- `Content-Language` : la langue naturelle ou les langues du contenu.
- `Content-Length` : la taille en octets du corps de la réponse.
- `Content-Location` : une URI spécifiant une ressource alternative pour laquelle cette réponse est pertinente.
- `Content-Type` : le type MIME du corps de la réponse.
- `Expires` : donne la date et l'heure après lesquelles la réponse n'est plus valide.
- `Last-Modified` : la date et l'heure à laquelle la ressource a été modifiée pour la dernière fois.

Il existe également d'autres en-têtes qui peuvent être renvoyés en réponse à une requête GET, mais cela dépend du serveur Web et de l'application Web utilisée.

////////////////  EN TETE REQUETE DELETE
Voici une liste d'en-têtes couramment utilisés pour une requête de méthode DELETE :

- **Host** : ce champ indique le nom de domaine du serveur sur lequel la ressource demandée doit être supprimée.

- **User-Agent** : cet en-tête facilite l'identification du navigateur ou de l'application cliente qui a initié la demande.

- **Accept** : cette en-tête spécifie les formats MIME acceptables pour la réponse.

- **Authorization** : si la requête nécessite une identification, cet en-tête est utilisé pour fournir les informations d'authentification.

- **Content-Type** : si une charge utile est incluse dans la demande, ce champ spécifie le type de données contenues dans la charge utile.

- **Content-Length** : si une charge utile est incluse dans la demande, ce champ spécifie la longueur de la charge utile en octets.

Notez que ces en-têtes sont facultatifs et peuvent ne pas être inclus dans toutes les requêtes DELETE.

//////////////// 2 types de requetes DELETE : 1 - DELETE FILE / 2 - DELETE USER IN BDD
Voici un exemple de requête DELETE demandant la suppression d'un fichier :

DELETE /documents/mon_fichier.doc HTTP/1.1
Host: www.example.com
Authorization: Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ==
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:120.0) Gecko/20100101 Firefox/120.0
Accept-Language: en-US,en;q=0.5
Connection: keep-alive

Et voici un exemple de requête DELETE demandant la suppression d'un utilisateur dans une base de données :

DELETE /users/123 HTTP/1.1
Host: www.example.com
Authorization: Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ==
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:120.0) Gecko/20100101 Firefox/120.0
Accept-Language: en-US,en;q=0.5
Connection: keep-alive

Notez que l'en-tête `Authorization` est utilisé pour s'authentifier auprès du serveur pour pouvoir effectuer cette opération. Aussi, le chemin diffère selon ce que nous souhaitons supprimer.

////////////////  Voici deux exemples de requêtes GET:

1. Requête pour obtenir la page d'accueil d'un site web:

GET / HTTP/1.1
Host: www.example.com

2. Requête pour obtenir un objet JSON depuis une API:

GET /api/users/123 HTTP/1.1
Host: api.example.com
Accept: application/json
Authorization: Bearer <token>

////////////////  Voici trois exemples de requête POST :

1. Requête POST pour ajouter un utilisateur à la base de données

POST /api/users HTTP/1.1
Host: example.com
Content-Type: application/json
Content-Length: 52

{"name": "John Doe", "email": "johndoe@example.com"}

2. Requête POST pour envoyer des données de formulaire

POST /submit-form.php HTTP/1.1
Host: example.com
Content-Type: application/x-www-form-urlencoded
Content-Length: 27

username=johndoe&password=secret

3. Requête POST pour envoyer un fichier utilisant multipart/form-data

POST /upload HTTP/1.1
Host: www.example.com
Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryuL67FWkv1CA

------WebKitFormBoundaryuL67FWkv1CA
Content-Disposition: form-data; name="file"; filename="example.txt"
Content-Type: text/plain

Hello World!
------WebKitFormBoundaryuL67FWkv1CA--

//////////////// 

Voici les principaux éléments pouvant être présents dans une requête HTTP et leur signification :

1. **Méthode** : il s'agit de l'action à réaliser sur la ressource identifiée par l'URI. Il existe plusieurs méthodes standards (GET, POST, PUT, DELETE, HEAD...) ainsi que des méthodes étendues.

2. **URI** : l'Uniform Resource Identifier, qui identifie la ressource à récupérer ou à modifier.

3. **Headers** : un ensemble d'entêtes fournissant des informations supplémentaires sur la requête, comme le type de contenu attendu ou la langue préférée.

4. **Corps** : optionnel, il contient les données associées à la requête, telles qu'un formulaire rempli ou des fichiers à télécharger.

5. **Paramètres de requête** : optionnels, ils permettent d'envoyer des données supplémentaires dans l'URL.

6. **Cookies** : optionnels, ils permettent à un serveur d'associer des informations à un client.

7. **Protocole** : il s'agit de la version du protocole HTTP utilisée pour cette requête (HTTP/1.0, HTTP/1.1, HTTP/2...).

8. **Sécurité** : si la connexion est sécurisée, la requête peut inclure un certificat SSL/TLS pour assurer la confidentialité et l'authenticité des données échangées.

9. **Authentification** : optionnelle, elle permet d'identifier le client auprès du serveur, généralement en utilisant un nom d'utilisateur et un mot de passe.

//////////////// 

Voici un exemple de requête GET :

GET /search?q=example HTTP/1.1
Host: www.example.com
Connection: keep-alive
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 11_2_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.82 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Sec-Fetch-Site: none
Sec-Fetch-Mode: navigate
Sec-Fetch-User: ?1
Sec-Fetch-Dest: document
Accept-Encoding: gzip, deflate, br
Accept-Language: en-US,en;q=0.9


Et voici un exemple de requête POST :

POST /login HTTP/1.1
Host: example.com
Content-Type: application/x-www-form-urlencoded
Content-Length: 25

username=johndoe&password=1234

Ceci est une requête GET qui demande à un serveur web de retourner les résultats d'une recherche pour la chaîne de caractères "example". Le reste de l'en-tête donne des informations supplémentaires sur la requête :
- Host: www.example.com : indique le nom de domaine auquel la requête est destinée
- Connection: keep-alive : spécifie que la connexion doit être maintenue ouverte après la réponse, dans le but de réduire le temps de latence lors de futures requêtes
- Upgrade-Insecure-Requests: 1 : indique au serveur qu'il peut utiliser HTTPS pour servir cette requête à la place de HTTP standard
- User-Agent: : identifie le type de navigateur utilisé par l'utilisateur
- Accept: définit les types MIME acceptés en réponse à la requête
- Sec-Fetch-Site, Sec-Fetch-Mode, Sec-Fetch-User, et Sec-Fetch-Dest : des en-têtes ajoutés pour améliorer la sécurité des requêtes en évitant les attaques CSRF et en empêchant les fuites d'informations utilisateur via les en-têtes Referrer
- Accept-Encoding : indique les types de compression de contenu supportés par le client
- Accept-Language : indique les langues préférées de l'utilisateur pour les réponses


//////////////// CODE ERREURS HTTP


Les codes d'erreurs HTTP (ou status codes) sont utilisés par les serveurs pour répondre aux requêtes des clients. Les codes d'erreurs commencent tous par un chiffre indiquant une catégorie: 
- 1xx : Informationnel
- 2xx : Succès
- 3xx : Redirection
- 4xx : Erreur client
- 5xx : Erreur serveur

Voici une liste de certains des codes d'erreurs HTTP ainsi qu'une brève description :

- **200 OK** : la requête a été réalisée avec succès.
- **201 Created** : la requête a été réalisée avec succès, une nouvelle ressource a été créée.
- **204 No Content** : la requête a été réalisée avec succès, mais il n'y a pas de contenu à renvoyer.
- **301 Moved Permanently** : l'URI de la ressource demandée a été modifié. La nouvelle URI est fournie en réponse.
- **302 Found** : l'URI de la ressource demandée a été temporairement déplacé vers un autre emplacement fourni en réponse.
- **304 Not Modified** : la ressource n'a pas été modifiée depuis la dernière demande. Aucune donnée n'est retournée.
- **400 Bad Request** : la requête du client était mal formée ou invalide.
- **401 Unauthorized** : le client doit s'authentifier pour accéder à la ressource.
- **403 Forbidden** : le client est authentifié mais n'a pas les droits d'accès nécessaires à la ressource demandée.
- **404 Not Found** : la ressource demandée n'a pas été trouvée sur le serveur.
- **500 Internal Server Error** : erreur interne du serveur. Cette réponse est générique et peut être renvoyée dans plusieurs situations différentes.
- **503 Service Unavailable** : Le serveur n'est pas disponible temporairement. Cela peut être dû à une maintenance programmée ou à une charge trop élevée sur le serveur.

Il existe d'autres codes d'erreurs HTTP qui peuvent indiquer différents problèmes rencontrés lors de l'utilisation d'un serveur web. Les clients HTTP (navigateurs web, etc.) traitent chacun de ces codes différemment, ce qui permet aux utilisateurs de savoir si leur requête a été réalisée avec succès ou non.