default: test.c driver.c i2c.c
	gcc -Wall -Wextra test.c driver.c i2c.c -o test -lm