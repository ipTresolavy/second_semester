library IEEE;
use IEEE.std_logic_1164.all;

entity carrega_3 is
port(
    x   : in std_logic;
    clk : in std_logic;

    z   : out std_logic
);
end entity carrega_3;

architecture moore_arch of carrega_3 is

    signal y, yinv, D   : std_logic_vector(2 downto 0);
    
    component D_flip_flop is
        generic ( initial: std_logic );
        port(
            D       : in std_logic;
            clk     : in std_logic;

            Q, Qinv : out std_logic
        );
    end component D_flip_flop;

begin
    D(2) <= (not x) and yinv(2) and y(1) and yinv(0);
    D(1) <= x and ( y(0) or ( yinv(2) and y(1) and yinv(0) ) );
    D(0) <= x and ( (y(2) xnor y(1)) or (y(2) and yinv(1) and yinv(0)) );

    ff_bundle: for i in y'range generate
        Dffs: D_flip_flop 
        generic map 
        (
            initial => '0'
        )
        port map
        (   D => D(i),
            clk => clk,
                        
            Q => y(i),
            Qinv => yinv(i)
        );
    end generate ff_bundle; -- ff_bundle

    z <= y(2) and yinv(1) and yinv(0);

end architecture moore_arch;

architecture mealy_arch of carrega_3 is

    signal y, yinv, D   : std_logic_vector(1 downto 0);
    
    component D_flip_flop is
        generic ( initial: std_logic );
        port(
            D       : in std_logic;
            clk     : in std_logic;

            Q, Qinv : out std_logic
        );
    end component D_flip_flop;

begin
    D(1) <= x and (y(1) or y(0));
    D(0) <= x and yinv(1);

    ff_bundle: for i in y'range generate
        Dffs: D_flip_flop 
        generic map 
        (
            initial => '0'
        )
        port map
        (   D => D(i),
            clk => clk,
                        
            Q => y(i),
            Qinv => yinv(i)
        );
    end generate ff_bundle; -- ff_bundle

    z <= (not x) and y(1) and yinv(0);

end architecture mealy_arch;