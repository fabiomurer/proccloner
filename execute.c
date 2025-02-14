#include "globals.h"
#include "execute.h"

void transfer_execution(void) {
    // TODO: set floating point registers
    __asm__ volatile (
        // Load general purpose registers from saved_regs.
        "movq saved_regs+0(%rip),  %r15\n\t"   // r15
        "movq saved_regs+8(%rip),  %r14\n\t"   // r14
        "movq saved_regs+16(%rip), %r13\n\t"   // r13
        "movq saved_regs+24(%rip), %r12\n\t"   // r12
        "movq saved_regs+32(%rip), %rbp\n\t"   // rbp
        "movq saved_regs+40(%rip), %rbx\n\t"   // rbx
        "movq saved_regs+48(%rip), %r11\n\t"   // r11
        "movq saved_regs+56(%rip), %r10\n\t"   // r10 after is rewritten with rip, hopefully not a problem (is a scratch register)
        "movq saved_regs+64(%rip), %r9\n\t"    // r9
        "movq saved_regs+72(%rip), %r8\n\t"    // r8
        "movq saved_regs+80(%rip), %rax\n\t"   // rax
        "movq saved_regs+88(%rip), %rcx\n\t"   // rcx
        "movq saved_regs+96(%rip), %rdx\n\t"   // rdx
        "movq saved_regs+104(%rip), %rsi\n\t"  // rsi
        "movq saved_regs+112(%rip), %rdi\n\t"  // rdi
        // Note: saved_regs.orig_rax (offset 120) is typically not used to restore state.
        
        // Set the stack pointer from saved_regs.rsp (offset 152).
        "movq saved_regs+152(%rip), %rsp\n\t"
        
        // Load the new instruction pointer from saved_regs.rip (offset 128)
        "movq saved_regs+128(%rip), %r10\n\t"
        
        // Jump to the new instruction pointer. This transfers execution.
        "jmp *%r10\n\t"
    );
}