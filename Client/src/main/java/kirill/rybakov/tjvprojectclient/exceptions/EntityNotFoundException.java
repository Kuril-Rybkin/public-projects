package kirill.rybakov.tjvprojectclient.exceptions;

public class EntityNotFoundException extends RuntimeException {
    private String message = "No message";

    public EntityNotFoundException() {

    }

    public EntityNotFoundException(String message) {
        this.message = message;
    }

    public String getMessage() {
        return message;
    }
}
