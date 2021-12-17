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

architecture arch of control_unit is

    type fsm is (fetch, decode, HALTING, PUSHSP, POPPC, ADD, AND_I, OR_I, LOAD, NOT_I, FLIP, NOP,
                STORE, STORE_WRITE, POPSP, ADDSP, CALL, STORESP, LOADSP, IM1, IM_star);

    signal next_state, current_state : fsm := fetch;

    begin
        
        change_of_state: process(clock, reset) is
            begin
                if reset = '1' then
                    current_state <= fetch;
                elsif rising_edge(clock) then
                    current_state <= next_state;
                end if;
        end process change_of_state;


        determining_next_state: process
        
            alias IR_MSN               : bit_vector(3 downto 0) is instruction(7 downto 4);
            alias IR_LSN               : bit_vector(3 downto 0) is instruction(3 downto 0);
            alias IR_MSt               : bit_vector(2 downto 0) is instruction(7 downto 5);
            alias IR_MSb               : bit is instruction(7);

            variable is_im_star, is_halted  : boolean := false;

            procedure wait_mem(dowrite:boolean) is

                begin

                    if dowrite then
                        mem_we <= '1';
                    end if;
                    
                    mem_enable <= '1';
                    wait until mem_busy = '1';
                    
                    mem_we <= '0';
                    wait until mem_busy = '0';
                    
                    mem_enable <= '0';

            end procedure wait_mem;


            begin 
                    
                halted <= '0';
                while not is_halted loop
                    pc_en <= '0';
                    sp_en <= '0';
                    ir_en <= '0';
                    case current_state is

                        when fetch =>
                            mem_a_addr_src <= '1'; -- pc
                            mem_b_addr_src <= "00"; -- sp
                            wait_mem(false); -- memA_rdd <= pc and memB_rdd <= sp
                            alu_a_src <= "00"; -- sp
                            alu_b_src <= "00"; -- imm_shft
                            alu_shfimm_src <= '0'; -- 1
                            alu_op <= "001"; -- addition
                            pc_src <= '0'; -- ula_out
                            pc_en <= '1'; -- pc <= pc + 1
                            ir_en <= '1'; -- sp <= instruction
                            next_state <= decode;
                        
                        when decode =>
                            --if next_state = decode then maybe take this if out
                                alu_a_src <=  "01"; -- sp
                                alu_b_src <= "00"; -- imm_shfimm
                                alu_shfimm_src <= '1'; -- 4
                                alu_op <= "001"; -- ula_out = sp + 4
                                mem_a_addr_src <= '0'; -- mem_a_addr <= sp
                                mem_b_addr_src <= "11"; -- mem_b_addr <= ula_out

                                if IR_MSb = '0' then
                                    is_im_star := false;
                                    if IR_MSt = "000" then
                                        if IR_MSN = "0001" then -- ADDSP
                                            alu_b_src <= "11"; -- not ir[4] & ir[3:0]<<2
                                            wait_mem(false);
                                            next_state <= ADDSP;
                                        else
                                            case IR_LSN is
                                                when "0000" => -- HALTING
                                                    next_state <= HALTING;
                                                when "0010" => -- PUSHSP
                                                    -- wait_mem(false);
                                                    next_state <= PUSHSP; 
                                                when "0100" => -- POPPC
                                                    wait_mem(false);
                                                    next_state <= POPPC;
                                                when "0101" => -- ADD
                                                    wait_mem(false);
                                                    sp_en <= '1';
                                                    next_state <= ADD;
                                                when "0110" => -- AND
                                                    wait_mem(false);
                                                    sp_en <= '1';
                                                    next_state <= AND_I;
                                                when "0111" => -- OR
                                                    wait_mem(false);
                                                    sp_en <= '1';
                                                    next_state <= OR_I;
                                                when "1000" => -- LOAD
                                                    wait_mem(false);
                                                    next_state <= LOAD;
                                                when "1001" => -- NOT
                                                    wait_mem(false);
                                                    next_state <= NOT_I;
                                                when "1010" => -- FLIP
                                                    wait_mem(false);
                                                    next_state <= FLIP;
                                                when "1011" => -- NOP
                                                    wait_mem(false);
                                                    next_state <= NOP;
                                                when "1100" => -- STORE
                                                    wait_mem(false);
                                                    next_state <= STORE;
                                                when "1101" => -- POPSP
                                                    wait_mem(false);
                                                    next_state <= POPSP;
                                                when others =>
                                                    report "Instrução não existe!"
                                                        severity failure;
                                            end case;
                                        end if;
                                    else 
                                        case IR_MSt is
                                            when "001" => -- CALL
                                                alu_op <= "100"; -- ula_out = sp - 4
                                                sp_en <= '1'; -- sp <= sp - 4
                                                next_state <= CALL;
                                            when "010" => -- STORESP
                                                wait_mem(false);
                                                next_state <= STORESP;
                                            when "011" => -- LOADSP
                                                alu_b_src <= "11"; -- not ir[4] & ir[3:0]<<2
                                                wait_mem(false);
                                                next_state <= LOADSP;
                                            when others =>
                                                report "Instrução não existe!"
                                                    severity failure;
                                        end case; 
                                    end if;
                                else
                                    if is_im_star = false then -- IM1
                                        is_im_star := true;
                                        alu_op <= "100"; -- ula_out = sp - 4
                                        sp_en <= '1'; -- sp <= sp - 4
                                        next_state <= IM1;
                                    else
                                        next_state <= IM_star; -- IM_star
                                    end if;
                                end if;
                            --end if;
                        
                        when HALTING =>
                            halted <= '1';
                            is_halted := true;
                            next_state <= HALTING;

                        when PUSHSP =>
                            alu_a_src <=  "01"; -- sp
                            alu_b_src <= "00"; -- imm_shft
                            alu_shfimm_src <= '1'; -- 4
                            alu_op <= "100"; -- alu_out = sp - 4
                            mem_b_addr_src <= "11"; -- memB_addr <= ula_out
                            mem_b_wrd_src <= "10" ;-- sp_out
                            wait_mem(true);
                            sp_en <= '1'; --sp=sp-4
                            next_state <= fetch;

                        when POPPC =>
                            pc_src <= '1'; -- pc_in <= memA_rdd
                            pc_en <= '1';
                            next_state <= fetch;

                        when ADD =>
                            mem_b_addr_src <= "00"; -- sp_out
                            alu_a_src <= "11"; -- memA_rdd
                            alu_b_src <= "01"; -- alu_mem
                            alu_mem_src <= '1'; -- memB_rdd
                            alu_op <= "001"; -- addition
                            mem_b_wrd_src <= "00"; -- ula_out
                            wait_mem(true);
                            next_state <= fetch;

                        when AND_I =>
                            mem_b_addr_src <= "00"; -- sp_out
                            alu_a_src <= "11"; -- memA_rdd
                            alu_b_src <= "01"; -- alu_mem
                            alu_mem_src <= '1'; -- memB_rdd
                            alu_op <= "010"; -- addition
                            mem_b_wrd_src <= "00"; -- ula_out
                            wait_mem(true);
                            next_state <= fetch;

                        when OR_I =>
                            mem_b_addr_src <= "00"; -- sp_out
                            alu_a_src <= "11"; -- memA_rdd
                            alu_b_src <= "01"; -- alu_mem
                            alu_mem_src <= '1'; -- memB_rdd
                            alu_op <= "011"; -- addition
                            mem_b_wrd_src <= "00"; -- ula_out
                            wait_mem(true);
                            next_state <= fetch;
                        
                        when LOAD =>
                            mem_b_addr_src <= "01"; -- memB_addr <= memA_rdd
                            wait_mem(false);
                            mem_b_addr_src <= "00"; -- sp
                            mem_b_wrd_src <= "01"; -- memb_mem
                            mem_b_mem_src <= '1'; -- memB_rdd
                            wait_mem(true);
                            next_state <= fetch;

                        when NOT_I =>
                            mem_b_addr_src <= "00"; -- sp
                            mem_b_wrd_src <= "00"; -- ula_out
                            alu_a_src <= "11"; -- memA_rdd
                            alu_op <= "101"; -- not memA_rdd
                            wait_mem(true);
                            next_state <= fetch;

                        when FLIP =>
                            mem_b_addr_src <= "00"; -- sp
                            mem_b_wrd_src <= "00"; -- ula_out
                            alu_a_src <= "11"; -- memA_rdd
                            alu_op <= "110"; -- flip memA_rdd
                            wait_mem(true);
                            next_state <= fetch;

                        when NOP =>
                            next_state <= fetch;

                        when STORE =>
                            mem_b_addr_src <= "01"; -- memB_addr <= memA_rdd
                            mem_b_mem_src <= '1'; -- memB_rdd
                            mem_b_wrd_src <= "01"; -- memB_mem (= memB_rdd)
                            alu_a_src <= "01"; -- sp_out
                            alu_shfimm_src <= '1'; -- 4
                            alu_b_src <= "00"; -- imm_shft (= 4)
                            alu_op <= "001"; -- addition
                            sp_en <= '1'; -- sp <= sp + 4
                            next_state <= STORE_WRITE;

                        when STORE_WRITE =>
                            -- sp_en <= '0';
                            wait_mem(true);
                            alu_a_src <= "01"; -- sp_out
                            alu_shfimm_src <= '1'; -- 4
                            alu_b_src <= "00"; -- imm_shft (= 4)
                            alu_op <= "001"; -- addition
                            sp_en <= '1'; -- sp <= sp + 4
                            next_state <= fetch;

                        when POPSP =>
                            alu_a_src <= "11"; -- memA_rdd
                            alu_op <= "000"; --ula_out = memA_rdd
                            sp_en <= '1'; --sp = memA_rdd
                            next_state <= fetch;

                        when ADDSP =>
                            mem_b_addr_src <= "00"; -- sp
                            alu_a_src <= "11"; -- memA_rdd
                            alu_mem_src <= '1'; -- memB_rdd
                            alu_b_src <= "01"; -- alu_mem
                            alu_op <= "001"; --addition
                            mem_b_wrd_src <= "00"; -- ula_out
                            wait_mem(true);
                            next_state <= fetch;

                        when CALL =>
                            mem_b_addr_src <= "00"; -- sp
                            alu_op <= "000";
                            alu_a_src <= "00"; -- pc 
                            mem_b_wrd_src <= "00"; -- ula_out
                            wait_mem(true);
                            alu_op <= "111";
                            alu_b_src <= "10"; -- ir[4:0] << 5
                            pc_src <= '0';
                            pc_en <= '1';
                            next_state <= fetch;
            
                        when STORESP =>
                            mem_b_addr_src <= "11"; -- ula_out
                            alu_a_src <=  "01"; -- sp
                            alu_b_src <= "11"; -- not ir[4] & ir[3:0]<<2
                            alu_op <= "001"; -- ula_out = sp + (not ir[4] & ir[3:0])<<2
                            mem_b_mem_src <= '0'; -- memA_rdd
                            mem_b_wrd_src <= "01"; -- memb_mem
                            wait_mem(true);
                            alu_a_src <=  "01"; -- sp
                            alu_b_src <= "00"; -- imm_shfimm
                            alu_shfimm_src <= '1'; -- 4
                            alu_op <= "001"; -- ula_out = sp + 4
                            sp_en <= '1';
                            next_state <= fetch;

                        when LOADSP =>
                            alu_a_src <= "01"; -- sp
                            alu_shfimm_src <= '1'; -- 4
                            alu_b_src <= "00"; -- imm_shft
                            alu_op <= "100"; -- subtraction
                            mem_b_addr_src <= "11"; -- memB_addr <= sp_out - 4
                            mem_b_mem_src <= '1'; -- memB_rdd
                            mem_b_wrd_src <= "01";  -- memB_mem
                            wait_mem(true);
                            sp_en <= '1';
                            next_state <= fetch;

                        when IM1 =>
                            mem_b_addr_src <= "00"; -- sp_out
                            mem_b_wrd_src <= "11"; -- signExt(ir[6:0])
                            wait_mem(true);
                            next_state <= fetch;

                        when IM_star =>
                            mem_b_addr_src <= "00"; -- sp
                            alu_mem_src <= '0'; -- memA_rdd << 7 or ir[6:0]
                            alu_b_src <= "01"; -- alu_mem
                            alu_op <= "111";
                            mem_b_wrd_src <= "00"; -- ula_out
                            wait_mem(true);
                            next_state <= fetch;

                        when others =>
                            report "Estado não existe!"
                                severity failure;
                    end case;
                wait on current_state;
            end loop;
        end process;
end architecture;