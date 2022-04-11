   -- Unidade de Controle --

   library ieee;
   use ieee.numeric_bit.all;

   entity control_unit is
       port(
           clock, reset: in bit;
           pc_en, ir_en, sp_en,
           pc_src, mem_a_addr_src, mem_b_mem_src, alu_shfimm_src, alu_mem_src,
           mem_we, mem_enable: out bit;
           mem_b_addr_src, mem_b_wrd_src, alu_a_src, alu_b_src: out bit_vector(1 downto 0);
           alu_op: out bit_vector(2 downto 0);
           mem_busy: in bit;
           instruction: in bit_vector(7 downto 0);
           halted: out bit
       );
   end entity control_unit;

   architecture arch of control_unit is
       type state_type is (Fetch, Fetch_continue, Decode, Break,
           Pushsp, Pushsp_continue, Poppc, Add_And_Or, Add_And_Or_continue, SOMA_SP,
           Load, Load_continue, Not_Flip, Nop, Store, Popsp, Addsp,
           Call, Call_continue, Storesp, Loadsp, Im1, Im2, Read_Write);
       signal present_state: state_type;
       signal next_state: state_type := Fetch;
       signal future_state: state_type;
       signal im_state: bit := '0';
       signal write_time: bit := '0';
   begin

       -- state register --
       state: process(clock, reset)
       begin
           if(reset = '1') then
               present_state <= Fetch;
           elsif(rising_edge(CLOCK)) then
               present_state <= next_state;
           end if;
       end process state;

       -- Finite State Machine --
       fsm: process(mem_busy, instruction, present_state, im_state) is
       -- state register --
       begin
           case present_state is

               when Fetch =>
                   -- registers --
                   pc_en <= '0';
                   ir_en <= '0';
                   sp_en <= '0';
                   halted <= '0';
                   -- ir = mem[pc] --
                   mem_a_addr_src <= '1';
                   -- Fetch -> Read--
                   next_state <= Read_Write;
                   future_state <= Fetch_continue;
               when Fetch_continue =>
                   -- registers --
                   pc_en <= '1';
                   sp_en <= '0';
                   -- pc = pc + 1 --
                   alu_op <= "001";
                   alu_a_src <= "00";
                   alu_b_src <= "00";
                   alu_shfimm_src <= '0';
                   pc_src <= '0';
                   -- ir/instruction = mem[pc] --
                   ir_en <= '1';
                   -- Fetch -> Decode --
                   next_state <= Decode;

               when Decode =>
                   -- registers --
                   pc_en <= '0';
                   ir_en <= '0';
                   if(instruction(7) = '1') then
                       sp_en <= '0';
                       if(im_state = '0') then
                           -- Decode -> Execute --
                           next_state <= Im1;
                       else
                           -- memA_rdd = mem[sp] --
                           mem_a_addr_src <= '0';
                           -- Decode -> Read --
                           next_state <= Read_Write;
                           future_state <= Im2;
                       end if;
                   elsif(instruction(6 downto 5) = "11") then
                       im_state <= '0';
                       sp_en <= '0';
                       -- memB_rdd = mem[sp + (not(ir[4]) & ir[3:0]) << 2] --
                       mem_b_addr_src(1) <= '1';
                       alu_a_src <= "01";
                       alu_b_src <= "11";
                       alu_op <= "001";
                       -- Decode -> Read --
                       next_state <= Read_Write;
                       future_state <= Loadsp;
                   elsif(instruction(6 downto 5) = "10") then
                       im_state <= '0';
                       sp_en <= '0';
                       -- memA_rdd = mem[sp] --
                       mem_a_addr_src <= '0';
                       -- Decode -> Read --
                       next_state <= Read_Write;
                       future_state <= Storesp;
                   elsif(instruction(6 downto 5) = "01") then
                       im_state <= '0';
                       sp_en <= '1';
                       -- sp = sp - 4 --
                       alu_a_src <= "01";
                       alu_b_src <= "00";
                       alu_shfimm_src <= '1';
                       alu_op <= "100";
                       -- Decode -> Execute --
                       next_state <= Call;
                   elsif(instruction(4) = '1') then
                       im_state <= '0';
                       sp_en <= '0';
                       -- memA_rdd = mem[sp] --
                       mem_a_addr_src <= '0';
                       -- memB_rdd = mem[sp + ir[3:0]«2] --
                       mem_b_addr_src(1) <= '1';
                       alu_a_src <= "01";
                       alu_b_src <= "11";
                       alu_op <= "001";
                       -- Decode -> Read --
                       next_state <= Read_Write;
                       future_state <= Addsp;
                   elsif(instruction(3 downto 0) = "1101") then
                       im_state <= '0';
                       sp_en <= '0';
                       -- memA_rdd = mem[sp] --
                       mem_a_addr_src <= '0';
                       -- Decode -> Read --
                       next_state <= Read_Write;
                       future_state <= Popsp;
                   elsif(instruction(3 downto 0) = "1100") then
                       im_state <= '0';
                       sp_en <= '0';
                       -- memA_rdd = mem[sp] --
                       mem_a_addr_src <= '0';
                       -- memB_rdd = mem[sp + 4] --
                       mem_b_addr_src(1) <= '1';
                       alu_a_src <= "01";
                       alu_b_src <= "00";
                       alu_shfimm_src <= '1';
                       alu_op <= "001";
                       -- Decode -> Read --
                       next_state <= Read_Write;
                       future_state <= Store;
                   elsif(instruction(3 downto 0) = "1011") then
                       im_state <= '0';
                       sp_en <= '0';
                       next_state <= Nop;
                   elsif(instruction(3 downto 0) = "1010" or instruction(3 downto 0) = "1001") then
                       im_state <= '0';
                       sp_en <= '0';
                       -- memA_rdd = mem[sp] --
                       mem_a_addr_src <= '0';
                       -- Decode -> Read --
                       next_state <= Read_Write;
                       future_state <= Not_Flip;
                   elsif(instruction(3 downto 0) = "1000") then
                       im_state <= '0';
                       sp_en <= '0';
                       -- memA_rdd = mem[sp] --
                       mem_a_addr_src <= '0';
                       -- Decode -> Read --
                       next_state <= Read_Write;
                       future_state <= Load;
                   elsif(instruction(3 downto 0) = "0111" or instruction(3 downto 0) = "0110" or instruction(3 downto 0) = "0101") then
                       im_state <= '0';
                       sp_en <= '0';
                       -- memA_rdd = mem[sp] --
                       mem_a_addr_src <= '0';
                       -- memB_rdd = mem[sp + 4] --
                       mem_b_addr_src(1) <= '1';
                       alu_a_src <= "01";
                       alu_b_src <= "00";
                       alu_shfimm_src <= '1';
                       alu_op <= "001";
                       -- Decode -> Read --
                       next_state <= Read_Write;
                       future_state <= Add_And_Or;
                   elsif(instruction(3 downto 0) = "0100") then
                       im_state <= '0';
                       sp_en <= '0';
                       -- memA_rdd = mem[sp] --
                       mem_a_addr_src <= '0';
                       -- Decode -> Read --
                       next_state <= Read_Write;
                       future_state <= Poppc;
                   elsif(instruction(3 downto 0) = "0010") then
                       im_state <= '0';
                       sp_en <= '0';
                       -- Decode -> Execute --
                       next_state <= Pushsp;
                   elsif(instruction(3 downto 0) = "0000") then
                       sp_en <= '0';
                       im_state <= '0';
                       -- Decode -> Execute --
                       next_state <= Break;
                   end if;

               when Break =>
                   halted <= '1';
                   pc_en <= '0';
                   ir_en <= '0';
                   sp_en <= '0';
                   im_state <= '0';
                   next_state <= Break;

               when Pushsp =>
                   -- registers --
                   pc_en <= '0';
                   ir_en <= '0';
                   sp_en <= '0';
                   -- mem[sp - 4] = sp --
                   mem_b_wrd_src <= "10";
                   mem_b_addr_src(1) <= '1';
                   -- sp = sp - 4 --
                   alu_a_src <= "01";
                   alu_b_src <= "00";
                   alu_shfimm_src <= '1';
                   alu_op <= "100";
                   -- Execute -> Write --
                   write_time <= '1';
                   next_state <= Read_Write;
                   future_state <= Pushsp_continue;
               when Pushsp_continue =>
                   -- registers --
                   pc_en <= '0';
                   ir_en <= '0';
                   sp_en <= '1';
                   -- sp = sp - 4 --
                   alu_a_src <= "01";
                   alu_b_src <= "00";
                   alu_shfimm_src <= '1';
                   alu_op <= "100";
                   -- Execute -> Fetch --
                   next_state <= Fetch;

               when Poppc =>
                   -- registers --
                   pc_en <= '1';
                   ir_en <= '0';
                   sp_en <= '1';
                   -- pc = mem[sp] --
                   pc_src <= '1';
                   -- sp = sp + 4 --
                   alu_a_src <= "01";
                   alu_b_src <= "00";
                   alu_shfimm_src <= '1';
                   alu_op <= "001";
                   -- Execute -> Fetch --
                   next_state <= Fetch;
               -- ADD/AND/OR(Execute) --
               when Add_And_Or =>
                   -- registers --
                   pc_en <= '0';
                   ir_en <= '0';
                   sp_en <= '1';
                   -- sp = sp + 4 --
                   alu_a_src <= "01";
                   alu_b_src <= "00";
                   alu_shfimm_src <= '1';
                   alu_op <= "001";
                   -- Execute -> Fetch --
                   next_state <= Add_And_Or_continue;
               when Add_And_Or_continue =>
                   -- registers --
                   pc_en <= '0';
                   ir_en <= '0';
                   sp_en <= '0';
                   -- Realiza a operação lógica/aritmética --
                   mem_b_addr_src <= "00";
                   mem_b_wrd_src <= "00";
                   alu_a_src(1) <= '1';
                   alu_b_src <= "01";
                   alu_mem_src <= '1';
                   -- Baseado no OPcode decide qual das 3 operações deve realizar --
                   alu_op <= (not instruction(2) & instruction(1 downto 0));
                   -- Execute -> Write --
                   write_time <= '1';
                   next_state <= Read_Write;
                   future_state <= Fetch;
               -- sp = sp + 4, apenas isso --
               when SOMA_SP =>
                   -- registers --
                   pc_en <= '0';
                   ir_en <= '0';
                   sp_en <= '1';
                   -- sp = sp + 4 --
                   alu_a_src <= "01";
                   alu_b_src <= "00";
                   alu_shfimm_src <= '1';
                   alu_op <= "001";
                   -- Execute -> Fetch --
                   next_state <= Fetch;

               when Load =>
                   -- registers --
                   pc_en <= '0';
                   ir_en <= '0';
                   sp_en <= '0';
                   -- memB_rdd = mem[mem[sp]] --
                   mem_b_addr_src <= "01";
                   -- Execute -> Read --
                   next_state <= Read_Write;
                   future_state <= Load_continue;
               when Load_continue =>
                   -- registers --
                   pc_en <= '0';
                   ir_en <= '0';
                   sp_en <= '0';
                   -- mem[sp] = memB_rdd --
                   mem_b_addr_src <= "00";
                   mem_b_wrd_src <= "01";
                   mem_b_mem_src <= '1';
                   -- Execute -> Write --
                   write_time <= '1';
                   next_state <= Read_Write;
                   future_state <= Fetch;
               -- NOT/FLIP(Execute) --
               when Not_Flip =>
                   -- registers --
                   pc_en <= '0';
                   ir_en <= '0';
                   sp_en <= '0';
                   -- Realiza a operação lógica --
                   mem_b_addr_src <= "00";
                   mem_b_wrd_src <= "00";
                   alu_a_src(1) <= '1';
                   -- Baseado no OPcode decide qual das 2 operações deve realizar --
                   alu_op <= (not instruction(2) & instruction(1 downto 0));
                   -- Execute -> Write --
                   write_time <= '1';
                   next_state <= Read_Write;
                   future_state <= Fetch;

               when Nop =>
                   -- registers --
                   pc_en <= '0';
                   ir_en <= '0';
                   sp_en <= '0';
                   -- Nada acontece --
                   -- Execute -> Fetch --
                   next_state <= Fetch;

               when Store =>
                   -- registers --
                   pc_en <= '0';
                   ir_en <= '0';
                   sp_en <= '1';
                   -- mem[mem[sp]] = mem[sp + 4] --
                   mem_b_addr_src <= "01";
                   mem_b_wrd_src <= "01";
                   mem_b_mem_src <= '1';
                   -- sp = sp + 4 --
                   alu_a_src <= "01";
                   alu_b_src <= "00";
                   alu_shfimm_src <= '1';
                   alu_op <= "001";
                   -- Execute -> Write --
                   write_time <= '1';
                   next_state <= Read_Write;
                   future_state <= SOMA_SP;

               when Popsp =>
                   -- registers --
                   pc_en <= '0';
                   ir_en <= '0';
                   sp_en <= '1';
                   -- sp = mem[sp] --
                   alu_a_src(1) <= '1';
                   alu_op <= "000";
                   -- Execute -> Fetch --
                   next_state <= Fetch;

               when Addsp =>
                   -- registers --
                   pc_en <= '0';
                   ir_en <= '0';
                   sp_en <= '0';
                   -- mem[sp] = mem[sp] + mem[sp + ir[3:0]<<2] --
                   mem_b_addr_src <= "00";
                   mem_b_wrd_src <= "00";
                   alu_a_src(1) <= '1';
                   alu_b_src <= "01";
                   alu_mem_src <= '1';
                   alu_op <= "001";
                   -- Execute -> Write --
                   write_time <= '1';
                   next_state <= Read_Write;
                   future_state <= Fetch;

               when Call =>
                   -- registers --
                   pc_en <= '0';
                   ir_en <= '0';
                   sp_en <= '0';
                   -- mem[sp] = pc --
                   mem_b_addr_src <= "00";
                   mem_b_wrd_src <= "00";
                   alu_a_src <= "00";
                   alu_op <= "000";
                   -- Execute -> Write --
                   write_time <= '1';
                   next_state <= Read_Write;
                   future_state <= Call_continue;
               when Call_continue =>
                   -- registers --
                   pc_en <= '1';
                   ir_en <= '0';
                   sp_en <= '0';
                   -- pc = ir[4:0]«5 --
                   pc_src <= '0';
                   alu_b_src <= "10";
                   alu_op <= "111";
                   -- Execute -> Fetch --
                   next_state <= Fetch;

               when Storesp =>
                   -- registers --
                   pc_en <= '0';
                   ir_en <= '0';
                   sp_en <= '0';
                   -- mem[sp + (not(ir[4])&ir[3:0])«2] = memA_rdd --
                   mem_b_wrd_src <= "01";
                   mem_b_mem_src <= '0';
                   mem_b_addr_src(1) <= '1';
                   alu_a_src <= "01";
                   alu_b_src <= "11";
                   alu_op <= "001";
                   -- Execute -> Write --
                   write_time <= '1';
                   next_state <= Read_Write;
                   future_state <= SOMA_SP;

               when Loadsp =>
                   -- registers --
                   pc_en <= '0';
                   ir_en <= '0';
                   sp_en <= '1';
                   -- sp = sp - 4 --
                   alu_a_src <= "01";
                   alu_b_src <= "00";
                   alu_shfimm_src <= '1';
                   alu_op <= "100";
                   -- mem[sp-4] = memB_rdd --
                   mem_b_addr_src(1) <= '1';
                   mem_b_wrd_src <= "01";
                   mem_b_mem_src <= '1';
                   -- Execute -> Write --
                   write_time <= '1';
                   next_state <= Read_Write;
                   future_state <= Fetch;

               when Im1 =>
                   -- registers --
                   pc_en <= '0';
                   ir_en <= '0';
                   sp_en <= '1';
                   -- sp = sp - 4 --
                   alu_a_src <= "01";
                   alu_b_src <= "00";
                   alu_shfimm_src <= '1';
                   alu_op <= "100";
                   -- mem[sp] = signExt(ir[6:0]) --
                   mem_b_addr_src(1) <= '1';
                   mem_b_wrd_src <= "11";
                   -- Execute -> Write --
                   write_time <= '1';
                   next_state <= Read_Write;
                   future_state <= Fetch;
                   im_state <= '1';

               when Im2 =>
                   -- registers --
                   pc_en <= '0';
                   ir_en <= '0';
                   sp_en <= '0';
                   -- mem[sp] = mem[sp]«7 | ir[6:0] --
                   mem_b_addr_src <= "00";
                   mem_b_wrd_src <= "00";
                   alu_b_src <= "01";
                   alu_mem_src <= '0';
                   alu_op <= "111";
                   -- Execute -> Write --
                   write_time <= '1';
                   next_state <= Read_Write;
                   future_state <= Fetch;
                   im_state <= '1';
               --  Realiza a escrita/leitura --
               when Read_Write =>
                   -- registers --
                   pc_en <= '0';
                   ir_en <= '0';
                   sp_en <= '0';
                   -- controla os sinais de acesso a memória baseando-se no mem_busy --
                   if(rising_edge(mem_busy)) then
                       write_time <= '0';
                       mem_we <= '0';
                   elsif(mem_busy = '0') then
                       mem_enable <= '1';
                       if(write_time = '1') then
                          mem_we <= '1';
                       end if;
                   end if;
                   if(mem_busy'event and mem_busy = '0') then
                       mem_enable <= '0';
                       next_state <= future_state;
                   else
                       next_state <= Read_Write;
                   end if;
           end case;
       end process fsm;

end architecture arch;
