# Simple debugger

## Mini debugger Commands

- break
- continue
- next (step by step)
- previous (go to previous address)


## Commands to compile

To compile the debugger_template in debug mode
```sh
gcc -g debugger_template.c linenoise.c -o debugger_template
```

To compile the debugger_template in debug mode for VS CODE
```sh
gcc -g debugger_template.c linenoise.c -o a.out
```

To compile the debugger_template in run mode
```sh
gcc debugger_template.c linenoise.c -o debugger_template
```

To compile the basicHello in debug mode
```sh
gcc basicHello.c -o basicHello
```