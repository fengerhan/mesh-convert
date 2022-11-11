
source = $(wildcard *.cpp)
object = $(patsubst %.cpp, %.o, $(source))
CXXFLAGS =  -g -std=c++11
#LDFLAGS += $(LIBS) -lmfem -lgnutls -lAFEPack -ldeal_II -lgsl -lblas
LDFLAGS += $(LIBS) -lmfem 

all : main


%.o : %.cpp
	$(CXX) -c -o $@ $< -I./ -I/usr/local/include   $(CXXFLAGS)

main : $(object)
	$(CXX) -o $@ $(object) $(LDFLAGS) $(LIBS)
	-@rm -f AFEPack

clean :
	-rm -f $(object)
	-rm -f main


.PHONY : default clean 

