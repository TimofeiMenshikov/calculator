CXXFLAGS =  -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE -DPRINT_PROCESSOR_STACK
CXX = g++
OBJDIR = object_files
STKDIR = stack
ASM_OBJECTS = $(OBJDIR)/asm.o $(OBJDIR)/text.o  
DISASM_OBJECTS = $(OBJDIR)/disasm.o $(OBJDIR)/text.o
PROCESSOR_OBJECTS = $(OBJDIR)/processor.o $(OBJDIR)/run_processor.o $(OBJDIR)/stack.o $(OBJDIR)/print.o $(OBJDIR)/hash.o $(OBJDIR)/canary.o $(OBJDIR)/text.o

CANARY_PROTECTION = False
HASH_PROTECTION   = False
STACK_DUMP 		  = True
STDOUT_DUMP 	  = True
HASH_DEBUG 		  = True

ifeq ($(HASH_DEBUG), True)
	CXXFLAGS += -DHASH_DEBUG
endif

ifeq ($(CANARY_PROTECTION), True)
	CXXFLAGS += -DCANARY_PROTECTION
endif

ifeq ($(HASH_PROTECTION), True)
	CXXFLAGS += -DHASH_PROTECTION
endif

ifeq ($(STACK_DUMP), True)
	CXXFLAGS += -DSTACK_DUMP
endif

ifeq ($(STDOUT_DUMP), True)
	CXXFLAGS += -DSTDOUT_DUMP
endif



all: asm.exe disasm.exe processor.exe
	asm.exe
	disasm.exe
	processor.exe

asm.exe: $(ASM_OBJECTS)
	@echo $(CXX) $^ [CXXFLAGS] -o $@
	@$(CXX) $^ $(CXXFLAGS) -o $@

disasm.exe: $(DISASM_OBJECTS)
	@echo $(CXX) $^ [CXXFLAGS] -o $@
	@$(CXX) $^ $(CXXFLAGS) -o $@

processor.exe: $(PROCESSOR_OBJECTS)	
	@echo $(CXX) $^ [CXXFLAGS] -o $@
	@$(CXX) $^ $(CXXFLAGS) -o $@

$(OBJDIR)/%.o: %.cpp 
	@echo [CXX] [CXXFLAGS] -c $@ -o $<
	@$(CXX) $(CXXFLAGS) -c -o  $@ $< 

$(OBJDIR)/%.o: $(STKDIR)/%.cpp
	@echo [CXX] [CXXFLAGS] -c $@ -o $<
	@$(CXX) $(CXXFLAGS) -c -o  $@ $< 


.PHONY: clean_exe
clean_exe:
	@echo "all .exe files are cleaned"
	@echo ""
	
	rm -rf *.exe

.PHONY: clean_object_files
clean_object_files:
	@echo "all object files (.o) are cleaned"
	@echo ""
	
	rm -rf *.o 

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf *.exe

.PHONY: create_documentation
create_docs: 
	doxygen Doxyfile
	@echo ""
	@echo "documentation is updated"
