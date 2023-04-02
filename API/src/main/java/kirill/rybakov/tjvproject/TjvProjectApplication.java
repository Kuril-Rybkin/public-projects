package kirill.rybakov.tjvproject;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.autoconfigure.security.servlet.SecurityAutoConfiguration;

@SpringBootApplication(exclude = SecurityAutoConfiguration.class)
public class TjvProjectApplication {

	public static void main(String[] args) {
		SpringApplication.run(TjvProjectApplication.class, args);
	}

}
