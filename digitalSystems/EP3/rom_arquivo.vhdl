-------------------------------------------------------
--! @file rom_arquivo.vhdl
--! @brief Implementação de ROM leitora de arquivos em VHDL para EP3 da disciplina de Sistemas Digitais I
--! @author Igor Pontes Tresolavy (tresolavy@usp.br)
--! @date 2021-11-28
-------------------------------------------------------

library IEEE;
use IEEE.numeric_bit.all;
use std.textio.all;

entity rom_arquivo is
    port (
        addr: in bit_vector(3 downto 0);
        data: out bit_vector(7 downto 0)
    );
end entity rom_arquivo;

architecture read_file_only_memory of rom_arquivo is

    type mem_tipo is array(0 to 15) of bit_vector(7 downto 0);

    -- file-reader function --
    impure function inicializa(nome_do_arquivo : in string) return mem_tipo is

        file     arquivo  : text open read_mode is nome_do_arquivo;
        variable linha    : line;
        variable temp_bv  : bit_vector(7 downto 0);
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

    constant mem : mem_tipo := inicializa("conteudo_rom_ativ_02_carga.dat");

    begin

        data <= mem(to_integer(unsigned(addr)));

end architecture read_file_only_memory;