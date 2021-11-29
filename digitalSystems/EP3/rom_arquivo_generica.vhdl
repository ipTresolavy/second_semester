-------------------------------------------------------
--! @file rom_arquivo_generica.vhdl
--! @brief Implementação de ROM genérica leitora de arquivos em VHDL para EP3 da disciplina de Sistemas Digitais I
--! @author Igor Pontes Tresolavy (tresolavy@usp.br)
--! @date 2021-11-28
-------------------------------------------------------

library IEEE;
use IEEE.numeric_bit.all;
use std.textio.all;

entity rom_arquivo_generica is
    generic(
        addressSize : natural := 4;
        wordSize    : natural := 8;
        datFileName : string  := "rom.dat"
    );
    port (
        addr: in bit_vector(addressSize-1 downto 0);
        data: out bit_vector(wordSize-1 downto 0)
    );
end entity rom_arquivo_generica;

architecture generic_read_file_only_memory of rom_arquivo_generica is

    type mem_tipo is array(0 to 2**addressSize -1) of bit_vector(wordSize-1 downto 0);

    -- file-reader function --
    impure function inicializa(nome_do_arquivo : in string) return mem_tipo is
        
        file     arquivo  : text open read_mode is nome_do_arquivo;
        variable linha    : line;
        variable temp_bv  : bit_vector(wordSize-1 downto 0);
        variable temp_mem : mem_tipo;
            
            begin
                for i in mem_tipo'range loop
                    readline(arquivo, linha);
                    read(linha, temp_bv);
                    temp_mem(i) := temp_bv;
                end loop;

    return temp_mem;
    end;
    -- end of function --

    constant mem : mem_tipo := inicializa(datFileName);

    begin

        data <= mem(to_integer(unsigned(addr)));

end architecture generic_read_file_only_memory;