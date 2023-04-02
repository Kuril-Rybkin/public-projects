package kirill.rybakov.tjvprojectclient;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.autoconfigure.security.servlet.SecurityAutoConfiguration;

@SpringBootApplication(exclude = SecurityAutoConfiguration.class)
public class TjvProjectClientApplication {

	public static void main(String[] args) {
		SpringApplication.run(TjvProjectClientApplication.class, args);
	}

}
