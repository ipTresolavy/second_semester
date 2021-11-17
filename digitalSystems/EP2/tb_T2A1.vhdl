library IEEE;
use IEEE.numeric_bit.all;

entity tb_T2A1 is
end entity tb_T2A1;

architecture DUT of tb_T2A1 is

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

    signal A        : bit_vector(7 downto 0);
    signal B        : bit_vector(7 downto 0);
    signal S        : bit_vector(2 downto 0);
    signal F        : bit_vector(7 downto 0);

    signal Z, Ov, Co: bit;

begin
    ALU_DUT: entity work.alu(operation) port map (A,B,F,S,Z,Ov,Co);

    stim_proc: process
    begin
        S <= "100";
        wait for 10 ps;
        A <= "10011000";
        B <= "00010001";
        wait for 80 ps;
        assert F = "10000111"
            report "bad F 001.1" severity error;
        assert Z = '0'
            report "bad Z 001.2" severity error;
        assert Co = '1'
            report "bad Co 001.3" severity error;
        wait for 10 ps;
        A <= "00001001";
        B <= "00000101";
        wait for 90 ps;
        assert F = "00000100"
            report "bad F 001.4" severity error;
        assert Z = '0'
            report "bad Z 000.5" severity error;
        assert Co = '1'
            report "bad Co 000.6" severity error;
        wait for 10 ps;

        A <= "00000111";
        B <= "00001100";
        wait for 90 ps;
        assert F = "11111011"
            report "bad F 001.7" severity error;
        assert Z = '0'
            report "bad Z 000.8" severity error;
        assert Co = '0'
            report "bad Co 000.9" severity error;
        wait for 10 ps;
    
        wait;
    end process;    

end architecture DUT;