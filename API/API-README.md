# tjv-project-final: Ready for evaluation

## Dependencies:

- Spring
- H2 database (Used in both testing and runtime - the API does not use any external databases)

## Repository

The backend application is located on the **master** branch

The client application is located on the **client-master** branch

## Known bugs

- When running the Client from a terminal using `java -jar ./tjv-project-client-1.0.jar` (i.e, outside of Intellij) Thymeleaf cannot resolve the template `gallery/gallery`.

## Running

Both the API and the client can be run by any of the following ways:
- Cloning, checking out to the latest commit, then running the application in Intellij IDEA
- Cloning, checking out to the latest commit, and running `./gradlew bootRun`
- Downloading the compiled `.jar` artifact from the `compile` job in the `build` stage from the latest pipeline and running the command `java -jar [filename]`
- The pipeline also creates a docker container, however I have not had the opportunity to test it since this requires me to be on the faculty WiFi

## API Examples

Contained in the file `./src/test/resources/generated-requests.http`

## Complex query

`"SELECT i FROM Image i WHERE i.listing.category = :category"` in `ImageRepository.java`

Selects all images stored in the database, which are related to a listing with set category

## Business operation

Whenever retrieving a listing or a set of listings, the client also wants to retrieve the list of interested users to display alongside the listing. Therefore, every time the client calls an API GET call for a listing, it also calls an API GET call for the interested users in the listing.
