library ieee;
use ieee.numeric_bit.all;

-- PoliStack: Processador, memória não está feita --

    -- Fluxo de dados --
entity data_flow is
    generic(
        addr_s: natural := 16;
        word_s: natural := 32
    );
    port(
        clock, reset: in bit;
        memA_addr, memB_addr: out bit_vector(addr_s - 1 downto 0);
        memB_wrd: out bit_vector(word_s - 1 downto 0);
        memA_rdd, memB_rdd: in bit_vector(word_s - 1 downto 0);
        pc_en, ir_en, sp_en: in bit;
        pc_src, mem_a_addr_src, mem_b_mem_src: in bit;
        mem_b_addr_src, mem_b_wrd_src, alu_a_src, alu_b_src: in bit_vector(1 downto 0);
        alu_shfimm_src, alu_mem_src: in bit;
        alu_op: in bit_vector(2 downto 0);
        instruction: out bit_vector(7 downto 0)
    );
end entity data_flow;

entity mux2x1 is
    generic(
        size: natural := 32
    );
    port(
        A: in bit_vector(size - 1 downto 0);
        B: in bit_vector(size - 1 downto 0);
        S: in bit;
        Y: out bit_vector(size - 1 downto 0)
    );
end entity mux2x1;

architecture structural of mux2x1 is
begin
    Y <= A when S = '0' else
        B when S = '1' else
        A;
end architecture structural;

entity mux4x1 is
    generic(
        size: natural := 32
    );
    port(
        A: in bit_vector(size - 1 downto 0);
        B: in bit_vector(size - 1 downto 0);
        C: in bit_vector(size - 1 downto 0);
        D: in bit_vector(size - 1 downto 0);
        S: in bit_vector(1 downto 0);
        Y: out bit_vector(size - 1 downto 0)
    );
end entity mux4x1;

architecture structural of mux4x1 is
begin
    Y <= A when S = "00" else
        B when S = "01" else
        C when S = "10" else
        D when S = "11" else
        A;
end architecture structural;

architecture structural of data_flow is
    component d_register is
        generic(
            width: natural := 4;
            reset_value: natural := 0
        );
        port(
            clock, reset, load: in bit;
            d: in bit_vector(width - 1 downto 0);
            q: out bit_vector(width - 1 downto 0)
        );
    end component;
    component alu
        generic(
            size : natural := 8
        );
        port(
            A, B: in bit_vector(size - 1 downto 0);
            F: out bit_vector(size - 1 downto 0);
            S: in bit_vector(2 downto 0);
            Z: out bit;
            Ov: out bit;
            Co: out bit
        );
    end component;
    component mux2x1
        generic(
            size: natural := 32
        );
        port(
            A: in bit_vector(size - 1 downto 0);
            B: in bit_vector(size - 1 downto 0);
            S: in bit;
            Y: out bit_vector(size - 1 downto 0)
        );
    end component;
    component mux4x1
        generic(
            size: natural := 32
        );
        port(
            A: in bit_vector(size - 1 downto 0);
            B: in bit_vector(size - 1 downto 0);
            C: in bit_vector(size - 1 downto 0);
            D: in bit_vector(size - 1 downto 0);
            S: in bit_vector(1 downto 0);
            Y: out bit_vector(size - 1 downto 0)
        );
    end component;
    -- registers' signals --
    signal pc_entrada: bit_vector(word_s - 1 downto 0);
    signal pc_saida: bit_vector(word_s - 1 downto 0);
    signal pc_saida_addr: bit_vector(addr_s - 1 downto 0);
    signal ir_entrada: bit_vector(7 downto 0);
    signal ir_saida: bit_vector(7 downto 0);
    signal sp_saida: bit_vector(word_s - 1 downto 0);
    signal sp_saida_addr: bit_vector(addr_s - 1 downto 0);
    -- alu's signals --
    signal alu_a: bit_vector(word_s - 1 downto 0);
    signal alu_b: bit_vector(word_s - 1 downto 0);
    signal ir_alu_b_10: bit_vector(word_s - 1 downto 0);
    signal ir_alu_b_11: bit_vector(word_s - 1 downto 0);
    signal alu_saida: bit_vector(word_s - 1 downto 0);
    signal alu_saida_addr: bit_vector(addr_s - 1 downto 0);
    signal z: bit;
    signal ov: bit;
    signal co: bit;
    -- memA_addr's signal --
    signal memA_rdd_addr: bit_vector(addr_s - 1 downto 0);
    -- memB_wrd's signal --
    signal ir_b_wrd: bit_vector(word_s - 1 downto 0);
    -- memb_mem's signals --
    signal memb_mem: bit_vector(word_s - 1 downto 0);
    -- imm_shift's signals --
    signal imm_shift: bit_vector(word_s - 1 downto 0);
    signal increment_1: bit_vector(word_s - 1 downto 0);
    signal increment_4: bit_vector(word_s - 1 downto 0);
    -- alu_mem's signals --
    signal alu_mem: bit_vector(word_s - 1 downto 0);
    signal ir_alu_mem_mux: bit_vector(word_s - 1 downto 0);

begin

    -- registers and alu --

    PC: component d_register generic map(word_s, 0) port map(clock, reset, pc_en, pc_entrada, pc_saida);
    IR: component d_register generic map(8, 0) port map(clock, reset, ir_en, ir_entrada, ir_saida);
    SP: component d_register generic map(word_s, 16#1FFF8#) port map(clock, reset, sp_en, alu_saida, sp_saida);
    ULA: component alu generic map(word_s) port map(alu_a, alu_b, alu_saida, alu_op, z, ov, co);
    
    -- muxes --

    -- program counter --
    PC_mux: component mux2x1 generic map(word_s) port map(alu_saida, memA_rdd, pc_src, pc_entrada);

    -- memory --

    memA_addr_mux: component mux2x1 generic map(addr_s) port map(sp_saida_addr, pc_saida_addr, mem_a_addr_src, memA_addr);
    sp_saida_addr <= bit_vector(resize(unsigned(sp_saida), addr_s));
    pc_saida_addr <= bit_vector(resize(unsigned(pc_saida), addr_s));

    memB_addr_mux: component mux4x1 generic map(addr_s) port map(sp_saida_addr, memA_rdd_addr, alu_saida_addr, alu_saida_addr, mem_b_addr_src, memB_addr);
    memA_rdd_addr <= bit_vector(resize(unsigned(memA_rdd), addr_s));
    alu_saida_addr <= bit_vector(resize(unsigned(alu_saida), addr_s));

    memB_wrd_mux: component mux4x1 generic map(word_s) port map(alu_saida, memb_mem, sp_saida, ir_b_wrd, mem_b_wrd_src, memB_wrd);
    ir_b_wrd <= bit_vector(resize(unsigned(ir_saida(6 downto 0)), word_s));

    memB_mem_mux: component mux2x1 generic map(word_s) port map(memA_rdd, memB_rdd, mem_b_mem_src, memb_mem);

    -- alu --
    alu_a_mux: component mux4x1 generic map(word_s) port map(pc_saida, sp_saida, memA_rdd, memA_rdd, alu_a_src, alu_a);

    alu_b_mux: component mux4x1 generic map(word_s) port map(imm_shift, alu_mem, ir_alu_b_10, ir_alu_b_11, alu_b_src, alu_b);
    ir_alu_b_10 <= bit_vector(resize(unsigned(ir_saida(4 downto 0) & "00000"), word_s));
    ir_alu_b_11 <= bit_vector(resize(unsigned((not ir_saida(4)) & ir_saida(3 downto 0) & "00"), word_s));

    imm_shift_mux: component mux2x1 generic map(word_s) port map(increment_1, increment_4, alu_shfimm_src, imm_shift);
    increment_1 <= bit_vector(resize(to_unsigned(1, 2), word_s));
    increment_4 <= bit_vector(resize(to_unsigned(4, 4), word_s));

    alu_mem_mux: component mux2x1 generic map(word_s) port map(ir_alu_mem_mux, memB_rdd, alu_mem_src, alu_mem);
    ir_alu_mem_mux <= bit_vector(resize(unsigned(memA_rdd(word_s - 8 downto 0) & ir_saida(6 downto 0)), word_s));

    -- IR/Instruction --
    instruction <= ir_saida;
    ir_entrada <= bit_vector(resize(unsigned(sp_saida), 8));

end architecture structural;