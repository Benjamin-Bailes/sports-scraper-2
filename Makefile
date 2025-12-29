test1: test1.cpp
	clang++ test1.cpp -o test1 \
	  -I/opt/homebrew/opt/curl/include -L/opt/homebrew/opt/curl/lib -lcurl \
		-I/opt/homebrew/opt/gumbo-parser/include -L/opt/homebrew/opt/gumbo-parser/lib -lgumbo

misc: ./testing/misc.cpp retrievehtml.cpp parsehtml.cpp Race.cpp Horse.cpp
	clang++ ./testing/misc.cpp retrievehtml.cpp parsehtml.cpp Race.cpp Horse.cpp -o misc \
		-I/opt/homebrew/opt/gumbo-parser/include -L/opt/homebrew/opt/gumbo-parser/lib -lgumbo

main1: main.cpp retrievehtml.cpp parsehtml.cpp Race.cpp Horse.cpp
	clang++ main.cpp retrievehtml.cpp parsehtml.cpp Race.cpp Horse.cpp -o main1 \
			-I/opt/homebrew/opt/curl/include -L/opt/homebrew/opt/curl/lib -lcurl \
			-I/opt/homebrew/opt/gumbo-parser/include -L/opt/homebrew/opt/gumbo-parser/lib -lgumbo

# main2: main.cpp retrievehtml.cpp getlinks.cpp
# 	clang++ main.cpp retrievehtml.cpp getlinks.cpp -o main2 \
# 			-I/opt/homebrew/opt/curl/include -L/opt/homebrew/opt/curl/lib -lcurl \
#  			-I/opt/homebrew/opt/gumbo-parser/include -L/opt/homebrew/opt/gumbo-parser/lib -lgumbo

