library IEEE;
use IEEE.numeric_bit.all;

entity tb_T2A2 is
end entity tb_T2A2;

architecture DUT of tb_T2A2 is

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

    signal clock        : bit := '0';
    constant clock_period : time := 100 ps;

    signal reset, load  : bit;
    signal d, q         : bit_vector(7 downto 0);

begin
    register_DUT: entity work.d_register(memory) generic map (8, 85) port map (clock, reset, load, d, q);

    clock_process: process
    begin
        clock <= '0';
        wait for clock_period/2;
        clock <= '1';
        wait for clock_period/2;
    end process;

    stim_proc: process
    begin
        load <= '1';
        reset <= '1';
        wait for clock_period;
        assert q = "01010101"
            report "bad q 1" severity error;
        reset <= '0';
        d <= "10101010";
        wait for clock_period;
        assert q = "10101010"
            report "bad q 2" severity error;

        d <= "00000000";
        wait for clock_period/2;
        assert q = "10101010"
            report "bad q 3" severity error;

        d <= "11110000";
        wait for clock_period/2;
        assert q = "11110000"
            report "bad q 4" severity error;

        load <= '0';
        d <= "11111111";
        wait for clock_period;
        assert q = "11110000"
            report "bad q 5" severity error;

        reset <= '1';
        wait for clock_period;
        assert q = "01010101"
            report "bad q 6" severity error;
        
        wait;
    end process;    

end architecture DUT;