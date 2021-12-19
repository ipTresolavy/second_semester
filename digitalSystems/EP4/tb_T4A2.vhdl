library IEEE;
use IEEE.numeric_bit.all;
use IEEE.numeric_std.all;
use std.textio.all;
use IEEE.std_logic_1164.all;

entity tb_T4A2 is
end entity tb_T4A2;

architecture DUT of tb_T4A2 is
    
    component control_unit is
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
    end component control_unit;

        signal clock : bit := '0';
        signal reset :  bit := '1';
        constant clock_period : time := 2 ns;

        -- Memory interface
        signal mem_busy                    : bit := '0';
        signal halted                      : bit;

        -- Control unit interface
        signal pc_en, ir_en, sp_en           :  bit;
        signal pc_src, mem_a_addr_src,
               mem_b_mem_src, mem_we, mem_enable                 :  bit;
        signal mem_b_addr_src, mem_b_wrd_src,
               alu_a_src, alu_b_src          :  bit_vector(1 downto 0);
        signal alu_shfimm_src , alu_mem_src  :  bit;
        signal alu_op                        :  bit_vector(2 downto 0);
        signal instruction                   :  bit_vector(7 downto 0) := "00000000";

        type mem_type is array (0 to 32768-1) of bit_vector(32-1 downto 0);

        impure function init_mem(mif_file_name : in string) return mem_type is
            file mif_file : text open read_mode is mif_file_name;
            variable mif_line : line;
            variable temp_bv : bit_vector(32-1 downto 0);
            variable temp_mem : mem_type;
        begin
            for i in mem_type'range loop
                readline(mif_file, mif_line);
                read(mif_line, temp_bv);
                temp_mem(i) := temp_bv;
            end loop;
            return temp_mem;
        end;

        constant mem : mem_type := init_mem("hello_world.mif");
        signal inst : bit_vector(31 downto 0); 
    begin
    
        bruh: entity work.control_unit(arch) 
        port map(clock, reset,
         pc_en, ir_en, sp_en, pc_src, mem_a_addr_src, 
        mem_b_mem_src, alu_shfimm_src, alu_mem_src, mem_we, mem_enable, mem_b_addr_src, mem_b_wrd_src, alu_a_src, alu_b_src, 
        alu_op, mem_busy, instruction, halted);

        clock_process: process
        begin
        clock <= '0';
        wait for clock_period/2;
        clock <= '1';
        wait for clock_period/2;
        end process;

        mem_busy_process: process(mem_we) is
            begin
            if mem_we = '1' then
                mem_busy <= '0';
            else
                mem_busy <= '1';
            end if;
        end process;

        stim_proc: process(ir_en) is
            variable int : natural := 1;
            begin
                if ir_en = '1' then
                    instruction <= inst(7 downto 0);
                    int := int + 1;
                end if;
            end process;

end architecture DUT; 