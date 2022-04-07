################################################################################
# Building Object files 
################################################################################

# Pattern of the objects files 
OBJ_PATTERN = $(BUILDPATH)/%.o

# Change file extension from .c to .o
TMP1 =  $(SOURCES:%.c=$(OBJ_PATTERN))

# Change file extension from .cpp to .o
TMP2 = $(TMP1:%.cpp=$(OBJ_PATTERN))

# Final list ob object files to generate
OBJECTS = $(TMP2)

# Pattern of the objects files 
TEST_OBJ_PATTERN = $(BUILDPATH)/%.o

# Change file extension from .c to .o
TEST_OBJECTS = $(SOURCES:%.c=$(TEST_OBJ_PATTERN))

# Change file extension from .cpp to .o
TEST_OBJECTS += $(TEST_SOURCES:%.c=$(TEST_OBJ_PATTERN))

