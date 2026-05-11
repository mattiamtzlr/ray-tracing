SRC := $(shell find src -name "*.java")

run: compile
	@java -cp out com.raytracing.Main

compile: $(SRC)
	@mkdir -p out
	@javac -d out $(SRC)

clean:
	@rm -rf out
