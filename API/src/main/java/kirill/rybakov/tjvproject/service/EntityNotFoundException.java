package kirill.rybakov.tjvproject.service;

/**
 * Exception class thrown if Entities were not found
 */
public class EntityNotFoundException extends RuntimeException {
    public EntityNotFoundException() {

    }

    public EntityNotFoundException(String error) {
        this.message = error;
    }

    private String message = "";

    public String getMessage() {
        return message;
    }
}
