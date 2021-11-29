library IEEE;
use IEEE.numeric_bit.all;

entity tb_T3A2 is
end entity tb_T3A2;

architecture DUT of tb_T3A2 is

    component rom_arquivo is
        port (
            addr: in bit_vector(3 downto 0);
            data: out bit_vector(7 downto 0)
        );
    end component rom_arquivo;

    signal address        : bit_vector(3 downto 0);
    signal dataBundle     : bit_vector(7 downto 0);

begin
    ALU_DUT: entity work.rom_arquivo(read_file_only_memory) port map (address, dataBundle);

    stim_proc: process
    begin
        address <= "0000";
        wait for 10 ps;
        assert dataBundle = "00000001"
            report "bad dataBundle 1" severity error;

        address <= "0001";
        wait for 10 ps;
        assert dataBundle = "00000010"
            report "bad dataBundle 2" severity error;

        address <= "0010";
        wait for 10 ps;
        assert dataBundle = "00000100"
            report "bad dataBundle 3" severity error;

        address <= "0011";
        wait for 10 ps;
        assert dataBundle = "00001000"
            report "bad dataBundle 4" severity error;

        address <= "0100";
        wait for 10 ps;
        assert dataBundle = "00010000"
            report "bad dataBundle 5" severity error;

        address <= "0101";
        wait for 10 ps;
        assert dataBundle = "00100000"
            report "bad dataBundle 6" severity error;

        address <= "0110";
        wait for 10 ps;
        assert dataBundle = "01000000"
            report "bad dataBundle 7" severity error;

        address <= "0111";
        wait for 10 ps;
        assert dataBundle = "10000000"
            report "bad dataBundle 8" severity error;

        address <= "1000";
        wait for 10 ps;
        assert dataBundle = "10000000"
            report "bad dataBundle 9" severity error;

        address <= "1001";
        wait for 10 ps;
        assert dataBundle = "01000000"
            report "bad dataBundle 10" severity error;

        address <= "1010";
        wait for 10 ps;
        assert dataBundle = "00100000"
            report "bad dataBundle 11" severity error;

        address <= "1011";
        wait for 10 ps;
        assert dataBundle = "00010000"
            report "bad dataBundle 12" severity error;

        address <= "1100";
        wait for 10 ps;
        assert dataBundle = "00001000"
            report "bad dataBundle 13" severity error;

        address <= "1101";
        wait for 10 ps;
        assert dataBundle = "00000100"
            report "bad dataBundle 14" severity error;

        address <= "1110";
        wait for 10 ps;
        assert dataBundle = "00000010"
            report "bad dataBundle 15" severity error;

        address <= "1111";
        wait for 10 ps;
        assert dataBundle = "00000001"
            report "bad dataBundle 16" severity error;

        wait;
    end process;

end architecture DUT;