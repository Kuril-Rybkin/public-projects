package kirill.rybakov.tjvprojectclient.exceptions;

public class ErroneousInputException extends RuntimeException {
    private String message = "No message";

    public ErroneousInputException() {

    }

    public ErroneousInputException(String message) {
        this.message = message;
    }

    public String getMessage() {
        return message;
    }
}
