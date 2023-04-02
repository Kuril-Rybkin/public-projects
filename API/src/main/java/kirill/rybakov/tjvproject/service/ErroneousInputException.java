package kirill.rybakov.tjvproject.service;

/**
 * Exception class thrown if incorrect data is provided
 */
public class ErroneousInputException extends RuntimeException {
    public ErroneousInputException() {

    }

    public ErroneousInputException(String error) {
        this.message = error;
    }

    private String message = "";

    public String getMessage() {
        return message;
    }
}
