library IEEE;
use IEEE.numeric_bit.all;

entity tb_T4A1 is
end entity tb_T4A1;

architecture DUT of tb_T4A1 is
    
        component data_flow is
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
        end component data_flow;

        signal clock, reset :  bit := '0';
        constant clock_period : time := 50 ps;

        -- Memory interface
        signal memA_addr, memB_addr :  bit_vector(17-1 downto 0);
        signal memB_wrd             :  bit_vector(32-1 downto 0);
        signal memA_rdd, memB_rdd   :  bit_vector(32-1 downto 0);

        -- Control unit interface
        signal pc_en, ir_en, sp_en           :  bit;
        signal pc_src, mem_a_addr_src,
               mem_b_mem_src                 :  bit;
        signal mem_b_addr_src, mem_b_wrd_src,
               alu_a_src, alu_b_src          :  bit_vector(1 downto 0);
        signal alu_shfimm_src , alu_mem_src  :  bit;
        signal alu_op                        :  bit_vector(2 downto 0);
        signal instruction                   :  bit_vector(7 downto 0);
    
    begin
    
        bruh: entity work.data_flow(data_flow_beh) generic map (17, 32) 
        port map(clock, reset, memA_addr, memB_addr, memB_wrd, memA_rdd, memB_rdd,
         pc_en, ir_en, sp_en, pc_src, mem_a_addr_src, 
        mem_b_mem_src, mem_b_addr_src, mem_b_wrd_src, alu_a_src, alu_b_src, 
        alu_shfimm_src , alu_mem_src, alu_op, instruction);


        clock_process: process
        begin
        clock <= '0';
        wait for clock_period/2;
        clock <= '1';
        wait for clock_period/2;
        end process;

        stim_proc: process
            begin
                pc_en <= '1';
                ir_en <= '1';
                sp_en <= '1';
                pc_src <= '0';
                alu_op <= "001";
                alu_a_src <= "11";
                alu_b_src <= "01";
                alu_mem_src <= '1';
                mem_a_addr_src <= '1';
                memA_rdd <= "10110010011110001101001101110011";
                memB_rdd <= "01111010001000001100100100111110";
                wait for clock_period;
                pc_src <= '1';
                wait for clock_period;


                wait;
            end process;

end architecture DUT; 