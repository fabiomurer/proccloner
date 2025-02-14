# PROCCLONER

transform this program into another.

this is only for demostration porpueses so is not meant to run all programs, __for now only static binaries are supported (not all works)__.


## How it wotks

1. starts a program with address randomization and VDSO disabled and stop it right before its execution (linux and ld have loaded the program into memory)
2. copy the memory maps of the previous executed program from `/proc/<pid>/maps` and `/proc/<pid>/mem` and allocate into iteself (proccloner)
3. jumps to the othe program code by copying the other program cpu image into the cpu and performs a jump to `rip`


## compiling

```bash
make
```

## running the examples

```bash
make
cd example
make
cd ..

./proccloner ./example/hang
```

example of output
```
process with pid: 11499 is loaded and stopped
REGS:
        RIP: 0x4016e0
        RSP: 0x7fffffffdd80
        RBP: 0x0
        RAX: 0xffffffffffffffda
        RBX: 0x0
        RCX: 0x0
Extracted segment 0x400000-0x401000 (4096 bytes)
Mappend segment 0x400000-0x401000 (4096 bytes)
Extracted segment 0x401000-0x492000 (593920 bytes)
Mappend segment 0x401000-0x492000 (593920 bytes)
Extracted segment 0x492000-0x4bb000 (167936 bytes)
Mappend segment 0x492000-0x4bb000 (167936 bytes)
Extracted segment 0x4bb000-0x4c2000 (28672 bytes)
Mappend segment 0x4bb000-0x4c2000 (28672 bytes)
Extracted segment 0x4c2000-0x4c8000 (24576 bytes)
Mappend segment 0x4c2000-0x4c8000 (24576 bytes)
Failed to read memory from 0x7ffff7ff9000 to 0x7ffff7ffd000: Input/output error
Mappend segment 0x7ffff7ff9000-0x7ffff7ffd000 (16384 bytes)
Extracted segment 0x7ffff7ffd000-0x7ffff7fff000 (8192 bytes)
Mappend segment 0x7ffff7ffd000-0x7ffff7fff000 (8192 bytes)
Extracted segment 0x7ffffffde000-0x7ffffffff000 (135168 bytes)
Mappend segment 0x7ffffffde000-0x7ffffffff000 (135168 bytes)
pid: 11498, press any key to exit ...
```

process with pid `11498` is `proccloner`, pid `11499` is `./hang`, hang is loaded and killed, after (last line)
proccloner execute hang code, so the pid is of proccloner