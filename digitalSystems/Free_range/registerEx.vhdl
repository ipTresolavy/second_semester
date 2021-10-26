library IEEE;
use IEEE.std_logic_1164.all;

entity registerEx is
port(
    REG_IN  : in std_logic_vector(7 downto 0);
    LD      : in std_logic;
    clk     : in std_logic;

    REG_OUT  : out std_logic_vector(7 downto 0)
);
end entity registerEx;

architecture registerArch of registerEx is

begin

    reg_proc: process(clk)
    begin
        if LD /= '0' and rising_edge(clk) then
            REG_OUT <= REG_IN;
        end if;
    end process reg_proc;

end architecture registerArch;