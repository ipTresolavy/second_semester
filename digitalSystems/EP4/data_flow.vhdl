library IEEE;
use IEEE.numeric_bit.all;

entity data_flow is
    generic (
        addr_s : natural := 16; -- address size in bits
        word_s : natural := 32 -- word size in bits
    );
    port (
        clock, reset : in bit;
        
        -- Memory interface
        memA_addr, memB_addr : out bit_vector(addr_s-1 downto 0);
                    memB_wrd : out bit_vector(word_s-1 downto 0);
        memA_rdd, memB_rdd   : in bit_vector(word_s-1 downto 0);

        -- Control unit interface
        pc_en, ir_en, sp_en           : in bit;
        pc_src, mem_a_addr_src,
        mem_b_mem_src                 : in bit;
        mem_b_addr_src, mem_b_wrd_src,
        alu_a_src, alu_b_src          : in bit_vector(1 downto 0);
        alu_shfimm_src , alu_mem_src  : in bit;
        alu_op                        : in bit_vector(2 downto 0);
        instruction                   : out bit_vector(7 downto 0)
    );
end entity data_flow;

architecture data_flow_beh of data_flow is
    
    component alu is
        generic (
            size    : natural := 8
        );
        port (
            A, B    : in  bit_vector(size-1 downto 0); -- inputs
            F       : out bit_vector(size-1 downto 0); -- output
            S       : in  bit_vector(2 downto 0); -- op selection
            Z       : out bit; -- zero flag
            Ov      : out bit; -- overflow flag
            Co      : out bit -- carry out
        );
    end component alu;

    component d_register is
        generic(
            width: natural := 4;
            reset_value: natural := 0
        );
        port(
            clock, reset, load: in bit;
            d: in bit_vector(width-1 downto 0);
            q: out bit_vector(width-1 downto 0)
        );
    end component d_register;
    
        -- registers' inputs and outputs
        signal pc_in, pc_out, 
               sp_in, sp_out : bit_vector(word_s-1 downto 0);
        signal ir_out : bit_vector(7 downto 0);

        -- ALU's inputs and outputs
        signal A_in, B_in, ula_out  : bit_vector(word_s-1 downto 0);
        signal Z, Ov, Co            : bit;

        -- auxiliary signal for memB_wrd and B_in selection
        signal memb_mem, imm_shft, alu_mem : bit_vector(word_s-1 downto 0);

    begin

        alu_declaration: alu generic map (word_s) port map (A_in, B_in, ula_out, alu_op, Z, Ov, Co);

        program_counter: d_register generic map (word_s, 0) port map (clock, reset, pc_en, pc_in, pc_out);
        stack_pointer: d_register generic map (word_s, 131064) port map (clock, reset, sp_en, ula_out, sp_out);
        instruction_reg: d_register generic map (8, 0) port map (clock, reset, ir_en, memA_rdd(7 downto 0), ir_out);

        instruction <= ir_out;

        with pc_src select 
            pc_in <= ula_out when '0',
                     memA_rdd when others;

        with mem_a_addr_src select 
            memA_addr <= sp_out(addr_s-1 downto 0) when '0',
                         pc_out(addr_s-1 downto 0) when others;

        with mem_b_addr_src select 
            memB_addr <= sp_out(addr_s-1 downto 0) when "00",
                         memA_rdd when "01",
                         ula_out(addr_s-1 downto 0) when others;

        with mem_b_wrd_src select 
            memB_wrd <= ula_out when "00",
                        memb_mem when "01",
                        sp_out when "10",
                        bit_vector(to_signed(to_integer(signed(ir_out(6 downto 0))), word_s)) when others;

        with mem_b_mem_src select
            memb_mem <= memA_rdd when '0',
                        memB_rdd when others;

        with alu_a_src select
            A_in <= pc_out when "00",
                    sp_out when "01",
                    memA_rdd when others;

        with alu_b_src select
            B_in <= imm_shft when "00",
                    alu_mem when "01",
                    bit_vector(to_unsigned(to_integer(unsigned(ir_out(4 downto 0)  sll  5 )), word_s)) when "10",
                    bit_vector(to_unsigned(to_integer(unsigned(((((not ir_out(4)) & (ir_out(3 downto 0))) sll 2) ))), word_s)) when others;

        with alu_shfimm_src select
            imm_shft <= bit_vector(to_unsigned(1, word_s)) when '0',
                        bit_vector(to_unsigned(4, word_s)) when others;

        with alu_mem_src select
            alu_mem <= ( (memA_rdd sll 7) OR (bit_vector(to_unsigned(to_integer(unsigned(ir_out(6 downto 0))), word_s))) ) when '0',
                       memB_rdd when others;

end architecture data_flow_beh;