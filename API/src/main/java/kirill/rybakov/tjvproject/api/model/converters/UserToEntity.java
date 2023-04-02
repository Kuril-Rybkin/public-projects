package kirill.rybakov.tjvproject.api.model.converters;

import kirill.rybakov.tjvproject.api.model.UserDto;
import kirill.rybakov.tjvproject.domain.User;
import org.springframework.http.HttpStatus;
import org.springframework.stereotype.Component;
import org.springframework.web.server.ResponseStatusException;

import java.util.function.Function;

@Component
public class UserToEntity implements Function<UserDto, User> {
    /**
     * Converts UserDTO to User Entity
     * @param userDto the function argument UserDTO to be converted
     * @return Resulting User entity
     * @throws ResponseStatusException 400 BAD REQUEST if username is less than 3 characters long
     */
    @Override
    public User apply(UserDto userDto) {
        if (userDto.getUsername() == null || userDto.getUsername().length() < 3) {
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST);
        }
        if (userDto.getFullName() == null || userDto.getFullName().length() == 0) {
            userDto.setFullName("Anonymous");
        }
        if (userDto.getEmail() == null || userDto.getEmail().length() == 0) {
            userDto.setEmail("No email provided");
        }
        return new User(userDto.getUsername(), userDto.getFullName(), userDto.getEmail());
    }
}
