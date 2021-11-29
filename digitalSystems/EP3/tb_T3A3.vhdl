library IEEE;
use IEEE.numeric_bit.all;

entity tb_T3A3 is
end entity tb_T3A3;

architecture DUT of tb_T3A3 is

    component rom_arquivo_generica is
        generic(
            addressSize : natural := 4;
            wordSize    : natural := 8;
            datFileName : string  := "rom.dat"
        );
        port (
            addr: in bit_vector(addressSize-1 downto 0);
            data: out bit_vector(wordSize-1 downto 0)
        );
    end component rom_arquivo_generica;

    signal address        : bit_vector(1 downto 0);
    signal dataBundle     : bit_vector(15 downto 0);

begin
    ALU_DUT: entity work.rom_arquivo_generica(generic_read_file_only_memory) generic map(2, 16, "rom_test.dat") port map (address, dataBundle);

    stim_proc: process
    begin
        address <= "00";
        wait for 10 ps;
        assert dataBundle = "0000000011111111"
            report "bad dataBundle 1" severity error;

        address <= "01";
        wait for 10 ps;
        assert dataBundle = "0000001110101010"
            report "bad dataBundle 2" severity error;

        address <= "10";
        wait for 10 ps;
        assert dataBundle = "1100000000110000"
            report "bad dataBundle 3" severity error;

        address <= "11";
        wait for 10 ps;
        assert dataBundle = "0000110001010101"
            report "bad dataBundle 4" severity error;

        wait;
    end process;

end architecture DUT;