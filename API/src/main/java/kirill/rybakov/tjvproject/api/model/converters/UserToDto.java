package kirill.rybakov.tjvproject.api.model.converters;

import kirill.rybakov.tjvproject.api.model.UserDto;
import kirill.rybakov.tjvproject.domain.User;
import org.springframework.stereotype.Component;

import java.util.function.Function;

@Component
public class UserToDto implements Function<User, UserDto> {
    /**
     * Convert User to UserDTO
     * @param user the function argument User entity to be converted
     * @return Resulting UserDTO
     */
    @Override
    public UserDto apply(User user) {
        var ret = new UserDto();
        ret.setUsername(user.getUsername());
        ret.setFullName(user.getFullName());
        ret.setEmail(user.getEmail());

        return ret;
    }
}
