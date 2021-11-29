library IEEE;
use IEEE.numeric_bit.all;

entity tb_T3A1 is
end entity tb_T3A1;

architecture DUT of tb_T3A1 is

    component rom_simples is
        port (
            addr: in bit_vector(3 downto 0);
            data: out bit_vector(7 downto 0)
        );
    end component rom_simples;

    signal address        : bit_vector(3 downto 0);
    signal dataBundle     : bit_vector(7 downto 0);

begin
    ALU_DUT: entity work.rom_simples(read_only_memory) port map (address, dataBundle);

    stim_proc: process
    begin
        address <= "0000";
        wait for 10 ps;
        assert dataBundle = "00000000"
            report "bad dataBundle 1" severity error;

        address <= "0001";
        wait for 10 ps;
        assert dataBundle = "00000011"
            report "bad dataBundle 2" severity error;

        address <= "0010";
        wait for 10 ps;
        assert dataBundle = "11000000"
            report "bad dataBundle 3" severity error;

        address <= "0011";
        wait for 10 ps;
        assert dataBundle = "00001100"
            report "bad dataBundle 4" severity error;

        address <= "0100";
        wait for 10 ps;
        assert dataBundle = "00110000"
            report "bad dataBundle 5" severity error;

        address <= "0101";
        wait for 10 ps;
        assert dataBundle = "01010101"
            report "bad dataBundle 6" severity error;

        address <= "0110";
        wait for 10 ps;
        assert dataBundle = "10101010"
            report "bad dataBundle 7" severity error;

        address <= "0111";
        wait for 10 ps;
        assert dataBundle = "11111111"
            report "bad dataBundle 8" severity error;

        address <= "1000";
        wait for 10 ps;
        assert dataBundle = "11100000"
            report "bad dataBundle 9" severity error;

        address <= "1001";
        wait for 10 ps;
        assert dataBundle = "11100111"
            report "bad dataBundle 10" severity error;

        address <= "1010";
        wait for 10 ps;
        assert dataBundle = "00000111"
            report "bad dataBundle 11" severity error;

        address <= "1011";
        wait for 10 ps;
        assert dataBundle = "00011000"
            report "bad dataBundle 12" severity error;

        address <= "1100";
        wait for 10 ps;
        assert dataBundle = "11000011"
            report "bad dataBundle 13" severity error;

        address <= "1101";
        wait for 10 ps;
        assert dataBundle = "00111100"
            report "bad dataBundle 14" severity error;

        address <= "1110";
        wait for 10 ps;
        assert dataBundle = "11110000"
            report "bad dataBundle 15" severity error;

        address <= "1111";
        wait for 10 ps;
        assert dataBundle = "00001111"
            report "bad dataBundle 16" severity error;

        wait;
    end process;

end architecture DUT;