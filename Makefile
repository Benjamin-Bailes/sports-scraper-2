test1: test1.cpp
	clang++ test1.cpp -o test1 \
	  -I/opt/homebrew/opt/curl/include -L/opt/homebrew/opt/curl/lib -lcurl \
		-I/opt/homebrew/opt/gumbo-parser/include -L/opt/homebrew/opt/gumbo-parser/lib -lgumbo