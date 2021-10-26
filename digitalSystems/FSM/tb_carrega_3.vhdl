library IEEE;
use IEEE.std_logic_1164.all;

entity tb_carrega_3 is
end entity tb_carrega_3;

architecture tb of tb_carrega_3 is
    
    -- DUT
    -- component carrega_3 is
    --     port(
    --         x   : in std_logic;
    --         clk : in std_logic;
        
    --         z   : out std_logic
    --     );
    -- end component carrega_3;
    
    -- inputs
    signal x, clk   : std_logic := '0';
    
    -- output
    signal z1, z2        : std_logic;

    -- clock period
    constant clock_period : time := 100 ps;

begin

    -- DUT: carrega_3 port map
    -- (   x => x,
    --     clk => clk,
                            
    --     z => z
    -- );

    DUT_1: entity work.carrega_3(mealy_arch) port map
    (   x => x,
        clk => clk,
                            
        z => z1
    );

    DUT_2: entity work.carrega_3(moore_arch) port map
    (   x => x,
        clk => clk,
                            
        z => z2
    );

    clock_process: process
    begin
        clk <= '0';
        wait for clock_period/2;
        clk <= '1';
        wait for clock_period/2;
    end process;

    stim_proc: process
    begin
        wait for clock_period;
        x <= '1';
        wait for clock_period;
        x <= '0';
        wait for clock_period;
        x <= '1';
        wait for clock_period;
        x <= '1';
        wait for clock_period;
        x <= '0';
        wait for clock_period;
        x <= '1';
        wait for clock_period;
        x <= '1';
        wait for clock_period;
        x <= '1';
        wait for clock_period;
        x <= '0';
        wait for clock_period;
        x <= '1';
        wait for clock_period;
        x <= '1';
        wait for clock_period;
        x <= '1';
        wait for clock_period;
        x <= '1';
        wait for clock_period;
        x <= '0';
        wait;
    end process;

end architecture tb;