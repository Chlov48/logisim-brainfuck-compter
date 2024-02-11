CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra -Werror 

%: %.cpp
	@$(CXX) $(CXXFLAGS) $< -o $@
	@echo -e '**** COMPILATION REUSSIE **** \n'
	@echo -e '********* EXECUTION ********* \n'
	@./$@
	@echo -e '\n \n ******* FIN EXECUTION *******'
	@touch $<

clean:
	rm -f *.o *~ core 




