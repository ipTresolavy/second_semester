library IEEE;
use IEEE.std_logic_1164.all;

entity two_in_two_out is
port(
    D1_IN, D2_IN    : in std_logic_vector(7 downto 0);
    clk, LDA, LDB, SEL   : in std_logic;

    REG_A, REG_B    : out std_logic_vector(7 downto 0)

);
end entity two_in_two_out;

architecture complexEx of two_in_two_out is

    component registerEx is
        port(
            REG_IN  : in std_logic_vector(7 downto 0);
            LD      : in std_logic;
            clk     : in std_logic;
        
            REG_OUT  : out std_logic_vector(7 downto 0)
        );
        end component registerEx;

    component two_one_MUX is
        port(
            D1_IN, D2_IN    : in std_logic_vector(7 downto 0);
            SEL             : in std_logic;
            
            MUX_OUT         : out std_logic_vector(7 downto 0)
        );
        end component two_one_MUX;

        signal MUX_OUT  : std_logic_vector(7 downto 0);

begin

    mux: two_one_MUX port map ( D1_IN => D1_IN,
                                D2_IN => D2_IN,
                                SEL => SEL,

                                MUX_OUT => MUX_OUT);
    
    reg1: registerEx port map ( LD => LDA,
                                clk => clk,
                                REG_IN => MUX_OUT,
                                
                                REG_OUT => REG_A);
    
    reg2: registerEx port map ( LD => LDB,
                                clk => clk,
                                REG_IN => MUX_OUT,
                                
                                REG_OUT => REG_B);

end architecture complexEx;