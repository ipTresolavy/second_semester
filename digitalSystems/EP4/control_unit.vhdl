-------------------------------------------------------
--! @file control_unit.vhdl
--! @brief Unidade de controle para EP4 da disciplina de Sistemas Digitais I
--! @author Igor Pontes Tresolavy (tresolavy@usp.br)
--! @date 2021-12-12
-------------------------------------------------------
library IEEE;
use IEEE.numeric_bit.all;

entity control_unit is
    port (
        clock, reset : in bit;
        pc_en, ir_en, sp_en,
        pc_src, mem_a_addr_src, mem_b_mem_src, alu_shfimm_src, alu_mem_src,
        mem_we, mem_enable : out bit;
        mem_b_addr_src, mem_b_wrd_src, alu_a_src, alu_b_src : out bit_vector(1 downto 0);
        alu_op : out bit_vector(2 downto 0);
        mem_busy: in bit;
        instruction : in bit_vector(7 downto 0);
        halted: out bit
    );
end entity control_unit;